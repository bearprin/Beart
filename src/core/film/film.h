//
// Created by Bear on 2021/12/3.
//

#pragma once

#include "common.h"
namespace beart {
template<typename T>
class Film {
 public:
  Film(const unsigned int width, const unsigned int height) : width_(width), height_(height) {
    buffer_ = std::make_unique<T[]>(width_ * height_);
  }
  void set_color(const int &x, const int &y, const T &c) {
    this->buffer_[x * width_ + y] = c;
  }
  [[nodiscard]] T color(const int &x, const int &y) const {
    return buffer_[x * width_ + y];
  };

 private:
  unsigned int width_;
  unsigned int height_;
  std::unique_ptr<T[]> buffer_;
};
}

