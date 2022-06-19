#pragma once

#include <optional>

#include <png.h>

#include "image.hpp"

/** A simple wrapper of Image that interacts with libpng to load/store png files.
 */
class PNGFile {
    Image image_;
    std::unique_ptr<png_bytep[]> rowpointers_; // libpng rowpointers

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

    /** creates libpng rowpointers.
     *
     * libpng uses pointer array of pointers to contiguous memory (rows) to read/write file.
     * this way, we point to portions of single chunk of memory allocated by Image,
     * avoiding unnecessary numerous allocations and copying around, plus cache advantage.
     *
     * NOTE: this is on assumption that Image allocates single chunk.
     * NOTE: pointers are non-owning, so be careful and notified if Image reallocates.
     */
    std::unique_ptr<png_bytep[]> create_rowpointers() {
      auto rowpointers = std::make_unique<png_bytep[]>(image_.height());
      for (std::size_t y = 0; y < image_.height(); ++y)
        rowpointers[y] = image_.data() + (y * image_.width() * image_.channels());

      return rowpointers;
    }
};
