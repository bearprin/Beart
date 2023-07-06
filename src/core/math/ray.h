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
  Ray(Vec3f ori, Vec3f dir, unsigned int depth = 1, bool is_primary_ray = false)
      : ori_(std::move(ori)), dir_(std::move(dir)), depth(depth), is_primary_ray_(is_primary_ray) {
    t_min_ = 0.f;
    t_max_ = kMaxFloat;
  }
  Ray(Vec3f ori,
      Vec3f dir,
      unsigned int depth,
      float t_min = 0.f,
      float t_max = kMaxFloat,
      bool is_primary_ray = false)
      : ori_(std::move(ori)),
        dir_(std::move(dir)),
        depth(depth),
        t_min_(t_min),
        t_max_(t_max),
        is_primary_ray_(is_primary_ray) {
  }
  Vec3f operator()(const float &t) const {
    return ori_ + dir_ * t;
  }

  Vec3f ori_;
  Vec3f dir_;
  uint depth;
  float t_min_ = 0.f;  // the range of the ray
  float t_max_ = kMaxFloat;  // the range of the ray
  bool is_primary_ray_;
};
}