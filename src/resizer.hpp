#include "image.hpp"
#include "pngfile.hpp"

struct Resizer {
    static auto resize(const Image& image, std::size_t width, std::size_t height) -> std::optional<Image>;

    static auto resize(const PNGFile& image, std::size_t width, std::size_t height) -> std::optional<PNGFile> {
        auto resized = resize(image.raw_image(), width, height); // call resize(const Image&, ...)
        if (!resized)
            return std::nullopt;

        return PNGFile(std::move(*resized));
    }
};