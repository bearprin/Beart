//
// Created by Bear on 2023/7/6.
//

#pragma once

#include "common.h"
#include "vec.h"

namespace beart {
class Ray {
 public:
  Ray() = default;
  Ray(Vec3f ori,
      Vec3f dir,
      unsigned int depth = 0u,
      bool is_primary_ray = false,
      float t_min = 0.f,
      float t_max = kMaxFloat)
      : ori_(std::move(ori)),
        dir_(std::move(dir)),
        inv_dir_(1.f / dir_),
        depth_(depth),
        t_min_(t_min),
        t_max_(t_max),
        is_primary_ray_(is_primary_ray) {
    InitInvDirSign();
  }
  Vec3f operator()(const float &t) const {
    return ori_ + dir_ * t;
  }

  Vec3f ori_;
  Vec3f dir_;
  Vec3f inv_dir_;
  Vec3f inv_dir_pad_;
  int sign_[3]{0, 0, 0};
  uint depth_ = 0u;
  float t_min_ = 0.f;  // the range of the ray
  float t_max_ = kMaxFloat;  // the range of the ray
  bool is_primary_ray_;
 private:
  void InitInvDirSign() {
    inv_dir_pad_.x() = AddUlpMagnitude(inv_dir_.x(), 2);  // add ulp to avoid 0
    inv_dir_pad_.y() = AddUlpMagnitude(inv_dir_.y(), 2);  // add ulp to avoid 0
    inv_dir_pad_.z() = AddUlpMagnitude(inv_dir_.z(), 2);  // add ulp to avoid 0
    sign_[0] = inv_dir_.x() < 0;
    sign_[1] = inv_dir_.y() < 0;
    sign_[2] = inv_dir_.z() < 0;
  }
};
}