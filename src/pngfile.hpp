#pragma once

#include <string_view>
#include <optional>

#include <png.h>

#include "image.hpp"

// TODO maybe inherit Image?
class PNGFile {
    Image image_;
    std::unique_ptr<png_bytep[]> rowpointers_;

  public:
    static auto load_from_file(std::string_view filepath) -> std::optional<PNGFile>;
    static bool save_to_file(const Image& image, std::string_view filepath);

    PNGFile(const PNGFile&) = delete;
    PNGFile(PNGFile&&) noexcept = default;

    PNGFile(Image&& image) : image_(std::move(image)) {}

    Image& raw_image() noexcept { return image_; }

  private:
    // used by load_from_file static method.
    PNGFile(const std::size_t width, const std::size_t height)
      : image_(width, height)
      , rowpointers_(std::make_unique<png_bytep[]>(height))
    {
      for (std::size_t y = 0; y < image_.height(); ++y)
        rowpointers[y] = image.data() + (y * image_.width() * Image::channels);
    }
};