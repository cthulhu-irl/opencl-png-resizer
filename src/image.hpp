#pragma once

#include <cstddef>
#include <stdexcept>
#include <memory>
#include <iterator>

class Image {
    using seq_type = unsigned char;
    constexpr static std::size_t channels_count = 4;

    std::unique_ptr<seq_type[]> arr_ = nullptr;
    std::size_t                 width_ = 0;
    std::size_t                 height_ = 0;
    std::size_t                 size_ = 0;

  public:
    Image() noexcept {}

    Image(const std::size_t width, const std::size_t height)
        : arr_(std::make_unique<seq_type[]>(width * height * channels()))
        , width_(width)
        , height_(height)
        , size_(width * height * channels())
    {}

    void reset(const std::size_t width, const std::size_t height) {
        size_ = width * height * channels();
        width_ = width;
        height_ = height;
        arr_ = std::make_unique<seq_type[]>(size_);
    }

    std::size_t width() const noexcept { return width_; }
    std::size_t height() const noexcept { return height_; }
    std::size_t channels() const noexcept { return channels_count; }
    std::size_t size() const noexcept { return size_; }

    seq_type& at(const std::size_t x, const std::size_t y, const std::size_t channel) noexcept {
        return arr_[y * channels() * width_ + x * channels() + channel];
    }

    const seq_type& at(const std::size_t x, const std::size_t y, const std::size_t channel) const noexcept {
        return arr_[y * channels() * width_ + x * channels() + channel];
    }

    seq_type* data() noexcept { return static_cast<seq_type*>(arr_.get()); }
    const seq_type* data() const noexcept { return static_cast<const seq_type*>(arr_.get()); }

    seq_type* begin() noexcept { return data(); }
    seq_type* end() noexcept { return data() + size(); }
};
