#pragma once

#include <cstddef>
#include <stdexcept>
#include <memory>
#include <iterator>

class Image {
    using seq_type = unsigned char;

    std::unique_ptr<seq_type[]> arr_ = nullptr;
    std::size_t                 width_ = 0;
    std::size_t                 height_ = 0;
    std::size_t                 size_ = 0;

  public:
    static constexpr auto channels = 4; // RGBA

    Image() noexcept {}

    Image(const std::size_t width, const std::size_t height)
        : arr_(std::make_unique<seq_type[]>(width * height * channels))
        , width_(width)
        , height_(height)
        , size_(width * height * channels)
    {}

    void reset(const std::size_t width, const std::size_t height) {
        size_ = width * height * channels;
        width_ = width;
        height_ = height;
        arr_ = std::make_unique<seq_type[]>(width * height * channels);
    }

    auto width() const noexcept { return width_; }
    auto height() const noexcept { return height_; }
    auto size() const noexcept { return size_; }

    //seq_type& at(const std::size_t x, const std::size_t y) noexcept {
    //    return arr_[x * channels * width_ + y * channels];
    //}

    seq_type* data() noexcept { return arr_.get(); }

    seq_type* begin() noexcept { return data(); }
    seq_type* end() noexcept { return data() + size(); }
};
