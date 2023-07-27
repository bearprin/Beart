//
// Created by Bear on 2023/07/6.
//

#pragma once
#include "vec.h"
namespace beart {

struct LightSample {
  LightSample(const float u, const float v) : u_(u), v_(v) {}
  explicit LightSample(const Point2f &uv) : u_(uv.x()), v_(uv.y()) {}
  explicit LightSample(const Point2f &uv, const float t) : u_(uv.x()), v_(uv.y()), t_(t) {}
  float t_{};  // 1d sample

  // 2d sample
  float u_{};
  float v_{};
};
struct BsdfSample {
  BsdfSample(const float u, const float v) : u_(u), v_(v) {}
  explicit BsdfSample(const Point2f& uv) : u_(uv.x()), v_(uv.y()) {}
  explicit BsdfSample(const Point2f& uv, const float t) : u_(uv.x()), v_(uv.y()), t_(t) {}

  float t_{};  // 1d sample

  // 2d sample
  float u_{};
  float v_{};
};
struct PixelSample {
 public:
  float image_u_ = 0.f;
  float image_v_ = 0.f;
};

}