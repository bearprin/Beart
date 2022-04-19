//
// Created by Bear on 2021/12/3.
//

#pragma once

#include <utility>

#include "vec.h"
#include "transform.h"
#include "common.h"

namespace beart {
class Ray {
 public:
  Ray() = default;
  Ray(Vec3f ori, Vec3f dir, unsigned int depth = 1, bool is_primary_ray = false)
      : ori_(std::move(ori)), dir_(std::move(dir)), depth_(depth), is_primary_ray_(is_primary_ray) {
    t_min_ = 0.f;
    t_max_ = kMaxFloat;
    inv_dir_ = Vec3f{1.f / dir_.x(), 1.f / dir_.y(), 1.f / dir_.z()};
    sign_[0] = inv_dir_.x() < 0;
    sign_[1] = inv_dir_.y() < 0;
    sign_[2] = inv_dir_.z() < 0;
  }
  Ray(Vec3f ori,
      Vec3f dir,
      unsigned int depth,
      float t_min = 0.f,
      float t_max = kMaxFloat,
      bool is_primary_ray = false)
      : ori_(std::move(ori)),
        dir_(std::move(dir)),
        depth_(depth),
        t_min_(t_min),
        t_max_(t_max),
        is_primary_ray_(is_primary_ray) {
    inv_dir_ = Vec3f{1.f / dir_.x(), 1.f / dir_.y(), 1.f / dir_.z()};
    sign_[0] = inv_dir_.x() < 0;
    sign_[1] = inv_dir_.y() < 0;
    sign_[2] = inv_dir_.z() < 0;
  }
  Vec3f operator()(const float &t) const {
    return ori_ + dir_ * t;
  }
  [[nodiscard]] const Vec3f &ori() const {
    return ori_;
  }
  [[nodiscard]] const Vec3f &dir() const {
    return dir_;
  }
  [[nodiscard]] const Vec3f &inv_dir() const {
    return inv_dir_;
  }
  [[nodiscard]] const int *sign() const {
    return sign_;
  }
  [[nodiscard]] const float &t_min() const {
    return t_min_;
  };
  [[nodiscard]] const float &t_max() const {
    return t_max_;
  };
  [[nodiscard]] unsigned int depth() const {
    return depth_;
  }
  void set_dir(const Vec3f &dir) {
    dir_ = dir;
    inv_dir_ = Vec3f{1.f / dir_.x(), 1.f / dir_.y(), 1.f / dir_.z()};
    sign_[0] = inv_dir_.x() < 0;
    sign_[1] = inv_dir_.y() < 0;
    sign_[2] = inv_dir_.z() < 0;
  }
 private:
  Vec3f ori_;
  Vec3f dir_;
  Vec3f inv_dir_;
  int sign_[3];
  unsigned int depth_;
  float t_min_ = 0.f;  // the range of the ray
  float t_max_ = kMaxFloat;  // the range of the ray
  bool is_primary_ray_;
};
}