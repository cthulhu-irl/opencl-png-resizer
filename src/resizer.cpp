#include "resizer.hpp"

#include <iostream>

#define CL_HPP_ENABLE_EXCEPTIONS
#include <CL/cl2.hpp>

constexpr static const char* resizer_program_entry = "resize_linear";
constexpr static const char* resizer_program_src = R"eof(
__constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_NEAREST;

struct CLImageSize
{
   uint Width;
   uint Height;
};

__kernel void resize_linear(__read_only image2d_t source, __write_only image2d_t dest, struct CLImageSize src_img, struct CLImageSize dst_img, float ratioX, float ratioY)
{
   const int gx = get_global_id(0);
   const int gy = get_global_id(1);
   const int2 pos = { gx, gy };

   if (pos.x >= dst_img.Width || pos.y >= dst_img.Height)
      return;

   float2 srcpos = {(pos.x + 0.4995f) / ratioX, (pos.y + 0.4995f) / ratioY};
   int2 SrcSize = { (int)src_img.Width, (int)src_img.Height };

   float4 value;

   if ((int)(srcpos.x + .5f) == SrcSize.x)
      srcpos.x = SrcSize.x - 0.5001f;

   if ((int)(srcpos.y + .5f) == SrcSize.y)
      srcpos.y = SrcSize.y - 0.5001f;

   srcpos -= (float2)(0.5f, 0.5f);

   if (srcpos.x < -0.5f || srcpos.x >= SrcSize.x - 1 || srcpos.y < -0.5f || srcpos.y >= SrcSize.y - 1)
      value = 0;

   int x1 = (int)(srcpos.x);
   int x2 = (int)(srcpos.x + 1);
   int y1 = (int)(srcpos.y);
   int y2 = (int)(srcpos.y + 1);

   float factorx1 = 1 - (srcpos.x - x1);
   float factorx2 = 1 - factorx1;
   float factory1 = 1 - (srcpos.y - y1);
   float factory2 = 1 - factory1;

   float4 f1 = factorx1 * factory1;
   float4 f2 = factorx2 * factory1;
   float4 f3 = factorx1 * factory2;
   float4 f4 = factorx2 * factory2;

   const int2 pos0 = { x1, y1 };
   const int2 pos1 = { x2, y1 };
   const int2 pos2 = { x1, y2 };
   const int2 pos3 = { x2, y2 };

   float4 v1 = read_imagef(source, sampler, pos0);
   float4 v2 = read_imagef(source, sampler, pos1);
   float4 v3 = read_imagef(source, sampler, pos2);
   float4 v4 = read_imagef(source, sampler, pos3);
   value =  v1 * f1 + v2 * f2 + v3 * f3 + v4 * f4;

   write_imagef(dest, pos, value);
}
)eof";

struct CLImageSize { unsigned int Width; unsigned int Height; };

auto Resizer::resize(const Image& image, std::size_t width, std::size_t height)
    -> std::optional<Image>
{
     cl::Context context(CL_DEVICE_TYPE_DEFAULT);
     cl::CommandQueue queue(context, CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE);

     cl::Program program(context, resizer_program_src, true);

     Image output(width, height);

     CLImageSize inp_size { static_cast<unsigned int>(image.width()), static_cast<unsigned int>(image.height()) };
     CLImageSize out_size { static_cast<unsigned int>(output.width()), static_cast<unsigned int>(output.height()) };

     cl::ImageFormat image_format(CL_RGBA, CL_UNORM_INT8);

     // Create an OpenCL Image / texture and transfer data to the device
     cl::Image2D cl_image_in(
        context,
        CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
        image_format,
        image.width(),
        image.height(),
        0,
        (void *) image.data()
     );

     float ratio_x = static_cast<float>(output.width()) / static_cast<float>(image.width());
     float ratio_y = static_cast<float>(output.height()) / static_cast<float>(image.height());

     // Create a buffer for the result
     cl::Image2D cl_image_out(
        context,
        CL_MEM_WRITE_ONLY,
        image_format,
        output.width(),
        output.height(),
        0,
        nullptr
     );

     // Run kernel
     cl::KernelFunctor<cl::Image2D, cl::Image2D, CLImageSize, CLImageSize, float, float>
         resize_image(program, resizer_program_entry);

     resize_image(
         cl::EnqueueArgs(
             queue,
             cl::NDRange(
                 std::max(output.width(), image.width()),
                 std::max(output.height(), image.height())
             )
         ),
         cl_image_in, cl_image_out, inp_size, out_size, ratio_x, ratio_y
     );

     queue.enqueueReadImage(
         cl_image_out,
         CL_TRUE,
         {0, 0, 0},
         {output.width(), output.height(), 1},
         0,
         0,
         (void *) output.data()
    );

    return output;
}
