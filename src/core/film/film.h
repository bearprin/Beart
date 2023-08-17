//
// Created by Bear on 2023/7/7.
//

#pragma once

#include "common.h"
#include "spectrum.h"

#include <vector>
#include <memory>

//#include <stb_image_write.h>

//#define TINYEXR_USE_STB_ZLIB 1
//#define TINYEXR_IMPLEMENTATION
//#include <tinyexr.h>

namespace beart {
template<typename T>
class Film {
 public:
  using spectrum_t = T;

  Film(const unsigned int width, const unsigned int height) : width_(width), height_(height) {
    buffer_ = std::make_unique<T[]>(width_ * height_);
//    num_ = std::make_unique<unsigned int[]>(width_ * height_);
  }
  void set_color(const int &x, const int &y, const T &c) {
    this->buffer_[y * width_ + x] = c;
  }
//  void add_color(const int &x, const int &y, const T &c) {
//    num_[y * width_ + x]++;
//    this->buffer_[y * width_ + x] += c;
//  }
  T color(const int &x, const int &y) const {
    return buffer_[y * width_ + x];
  };

  bool Save(const std::string &filename) const;

 private:
  unsigned int width_;
  unsigned int height_;
//  std::unique_ptr<unsigned int[]> num_;
  std::unique_ptr<T[]> buffer_;
};
template<>
bool Film<RGBSpectrum>::Save(const std::string &filename) const;
}

