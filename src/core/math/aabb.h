//
// Created by Bear on 2023/7/6.
//

#pragma once

#include <utility>

#include "bbox.h"
namespace beart {
class AABB : public BBox {
 public:
  AABB() = default;
  AABB(Vec3f p_min, Vec3f p_max) : p_min_(std::move(p_min)), p_max_(std::move(p_max)) {}
  AABB(const AABB &aabb) : p_min_(aabb.p_min_), p_max_(aabb.p_max_) {}
  AABB(AABB &&other) : p_min_(std::move(other.p_min_)), p_max_(std::move(other.p_max_)) {}
  AABB &operator=(const AABB &other) {
    p_min_ = other.p_min_;
    p_max_ = other.p_max_;
    return *this;
  }
  BBoxType GetBBoxType() const override {
    return BBoxType::kAABB;
  }
  const Vec3f &operator[](uint i) const {
    return i == 0 ? p_min_ : p_max_;
  }

  float SurfaceArea() const override;
  float Volume() const override;
  bool Intersect(const Ray &ray) const override;
  bool IsInBox(const Point3f &p) const override;
 private:
  Vec3f p_min_;
  Vec3f p_max_;
};
}
