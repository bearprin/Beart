//
// Created by Bear on 2023/7/7.
//

#pragma once

#include "common.h"
#include "spectrum.h"

#include <vector>
#include <memory>

#include <stb_image_write.h>
namespace beart {
template<typename T>
class Film {
 public:
  using spectrum_t = T;

  Film(const unsigned int width, const unsigned int height) : width_(width), height_(height) {
    buffer_ = std::make_unique<T[]>(width_ * height_);
  }
  void set_color(const int &x, const int &y, const T &c) {
    this->buffer_[y * width_ + x] = c;
  }
  T color(const int &x, const int &y) const {
    return buffer_[y * width_ + x];
  };

  bool Save(const std::string &filename) const;

 private:
  unsigned int width_;
  unsigned int height_;
  std::unique_ptr<T[]> buffer_;
};
template<typename T>
bool Film<T>::Save(const std::string &filename) const {
  // get file extension
  auto extension = filename.find_last_of(".") != std::string::npos
                   ? filename.substr(filename.find_last_of(".") + 1)
                   : "";
  if (extension.empty()) {
    return false;
  }
  auto N = spectrum_t{}.size();
  // convert floating-point image to 8-bit per channel
  std::vector<uint8_t> data(width_ * height_ * N, 0);
  for (auto y = 0u; y < height_; ++y) {
    for (auto x = 0u; x < width_; ++x) {
      unsigned int pixel_offset = N * (y * width_ + x);
      auto cc = Clamp(LinearRGBTosRGB(buffer_[y * width_ + x]), 0.f, 1.f) * 255.f;

      data[pixel_offset + 0] = static_cast<uint8_t>(cc.data()[0]);
      data[pixel_offset + 1] = static_cast<uint8_t>(cc.data()[1]);
      data[pixel_offset + 2] = static_cast<uint8_t>(cc.data()[2]);
    }
  }
  if (extension == "png") {
    return stbi_write_png(filename.c_str(), width_, height_, N, &data[0],
                          sizeof(uint8_t) * width_ * N) != 0;
  } else if (extension == "jpg" || extension == "jpeg") {
    return stbi_write_jpg(filename.c_str(), width_, height_, N, &data[0], 100) != 0;
  } else if (extension == "bmp") {
    return stbi_write_bmp(filename.c_str(), width_, height_, N, &data[0]) != 0;
  } else if (extension == "tga") {
    return stbi_write_tga(filename.c_str(), width_, height_, N, &data[0]) != 0;
  }
}
}

