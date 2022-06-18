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
    static auto load_from_file(const char* filepath) -> std::optional<PNGFile>;
    static bool save_to_file(const PNGFile& image, const char* filepath);

    // TODO requires copy conversion constructor: PNGFile(const Image&)
    //static bool save_to_file(const Image& image, const char* filepath);

    PNGFile(const PNGFile&) = delete;
    PNGFile(PNGFile&&) noexcept = default;

    PNGFile(Image&& image)
      : image_(std::move(image))
      , rowpointers_(create_rowpointers())
    {}

    Image& raw_image() noexcept { return image_; }
    const Image& raw_image() const noexcept { return image_; }

    bool save_as(const char* filepath) { return save_to_file(*this, filepath); }

    auto width() const noexcept { return image_.width(); }
    auto height() const noexcept { return image_.height(); }

  private:
    // used by load_from_file static method.
    PNGFile(const std::size_t width, const std::size_t height)
      : image_(width, height)
      , rowpointers_(create_rowpointers())
    {}

    std::unique_ptr<png_bytep[]> create_rowpointers() {
      auto rowpointers = std::make_unique<png_bytep[]>(image_.height());
      for (std::size_t y = 0; y < image_.height(); ++y)
        rowpointers[y] = image_.data() + (y * image_.width() * Image::channels);

      return rowpointers;
    }
};