//
// Created by Bear on 2021/12/3.
//

#ifndef BEART_SRC_CORE_MATH_RAY_H_
#define BEART_SRC_CORE_MATH_RAY_H_

#include "Vec.h"
#include "Transform.h"

namespace beart {
class Ray {
 public:
  Ray() = default;
  Ray(Vec3f ori, Vec3f dir, unsigned int depth = 1, bool is_primary_ray = false)
      : ori_(std::move(ori)), dir_(std::move(dir)), depth_(depth), is_primary_ray(is_primary_ray) {
    t_min_ = 0.f;
    t_max_ = std::numeric_limits<float>::max();
  }
  Vec3f operator()(const float &t) const {
    return ori_ + dir_ * t;
  }

  Vec3f ori_;
  Vec3f dir_;
  unsigned int depth_;
  float t_min_;  // the range of the ray
  float t_max_;  // the range of the ray
  bool is_primary_ray;
};
}

#endif //BEART_SRC_CORE_MATH_RAY_H_
