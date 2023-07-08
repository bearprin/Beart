//
// Created by Bear on 2023/07/6.
//

#pragma once

namespace beart {

struct LightSample {
  float t_{};  // 1d sample

  // 2d sample
  float u_{};
  float v_{};
};
struct BsdfSample {
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