#include "resizer.hpp"
#include "openclmanager.hpp"

constexpr static const char* resizer_program_entry = "resize_nn";
constexpr static const char* resizer_program_src = R"eof(
__constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_NEAREST;

struct CLImageSize
{
   uint Width;
   uint Height;
};

__kernel void resize_nn(__read_only image2d_t source, __write_only image2d_t dest, struct CLImageSize src_img, struct CLImageSize dst_img, float ratioX, float ratioY)
{
   const int gx = get_global_id(0);
   const int gy = get_global_id(1);
   const int2 pos = { gx, gy };

   if (pos.x >= dst_img.Width || pos.y >= dst_img.Height)
      return;

   float2 srcpos = {(pos.x + 0.4995f) / ratioX, (pos.y + 0.4995f) / ratioY};
   int2 SrcSize = (int2)(src_img.Width, src_img.Height);

   float4 value;

   int2 ipos = convert_int2(srcpos);
   if (ipos.x < 0 || ipos.x >= SrcSize.x || ipos.y < 0 || ipos.y >= SrcSize.y)
      value = 0;
   else
      value = read_imagef(source, sampler, ipos);

   write_imagef(dest, pos, value);
}

)eof";

struct CLImageSize { unsigned int Width; unsigned Height; };

auto Resizer::resize(const Image& image, std::size_t width, std::size_t height)
    -> std::optional<Image>
{
    try {
        OpenCLManager manager{};
        manager.create_context();
        manager.create_kernel_program(resizer_program_src);

        Image output(width, height);

        CLImageSize inp_size { image.width(), image.height() };
        CLImageSize out_size { output.width(), output.height() };

        auto image_format = cl::ImageFormat(CL_RGBA, CL_UNORM_INT8);

        // Create an OpenCL Image / texture and transfer data to the device
        cl::Image2D cl_image_in = cl::Image2D(context_, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, image_format,
                                              image.width(), image.height(), 0, (void *) image.data());


        double ratio_x = static_cast<double>(in.width()) / static_cast<double>(out.width());
        double ratio_y = static_cast<double>(in.height()) / static_cast<double>(out.height());

        // Create a buffer for the result
        cl::Image2D cl_image_out = cl::Image2D(m_context, CL_MEM_WRITE_ONLY, image_format,
                                               output.width(), output.height(), 0, nullptr);

        // Run kernel
        cl::Kernel kernel = cl::Kernel(program_, resizer_program_entry);
        kernel.setArg(0, cl_image_in);
        kernel.setArg(1, cl_image_out);
        kernel.setArg(2, inp_size);
        kernel.setArg(3, out_size);
        kernel.setArg(4, ratio_x);
        kernel.setArg(5, ratio_y);

        m_queue.enqueueNDRangeKernel(
                kernel,
                cl::NullRange,
                cl::NDRange(output.width(), output.height()),
                cl::NullRange
        );

        cl::size_t<3> origin;
        cl::size_t<3> region;
        origin[0] = 0;
        origin[1] = 0;
        origin[2] = 0;
        region[0] = output.width();
        region[1] = output.height();
        region[2] = 1;

        m_queue.enqueueReadImage(cl_image_out, CL_TRUE, origin, region, 0, 0, (void *) output.data());
    }
    catch (cl::Error& err)
    {
        std::cerr << "Error running kernel: " << err.what() << " " << getCLErrorString(err.err()) << std::endl;
        return std::nullopt;
    }
}