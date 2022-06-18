#include <png.h>

#include "pngfile.hpp"

auto PNGFile::load_from_file(const char* filepath) -> std::optional<PNGFile> {
    FILE* fp = fopen(filepath);
    if (!fp)
        return std::nullopt;

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!png)
        return std::nullopt;

    png_infop info = png_create_info_struct(png);
    if(!info)
        return std::nullopt;

    if(setjmp(png_jmpbuf(png)))
        return std::nullopt;

    png_init_io(png, fp);

    png_read_info(png, info);

    auto width      = png_get_image_width(png, info);
    auto height     = png_get_image_height(png, info);
    auto color_type = png_get_color_type(png, info);
    auto bit_depth  = png_get_bit_depth(png, info);

    PNGFile output(width, height);

    if(bit_depth == 16)
        png_set_strip_16(png);

    if(color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);

    // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
    if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png);

    if(png_get_valid(png, info, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png);

    // These color_type don't have an alpha channel then fill it with 0xff.
    if(color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

    if(color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png);

    png_read_update_info(png, info);

    png_bytepp output.rowpointers_.get();
    png_read_image(png, rowpointers_);

    png_destroy_read_struct(&png, &info, NULL);
}

bool PNGFile::save_to_file(const Image& image, std::string_view filepath) {
    // TODO implement.
    return false;
}
