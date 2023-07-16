//
// Created by Bear on 2023/07/6.
//

#pragma once
#include "vec.h"
namespace beart {

struct LightSample {
  LightSample(float u, float v) : u_(u), v_(v) {}
  explicit LightSample(Point2f uv) : u_(uv.x()), v_(uv.y()) {}
  float t_{};  // 1d sample

  // 2d sample
  float u_{};
  float v_{};
};
struct BsdfSample {
  BsdfSample(float u, float v) : u_(u), v_(v) {}
  explicit BsdfSample(Point2f uv) : u_(uv.x()), v_(uv.y()) {}

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