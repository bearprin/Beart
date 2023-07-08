//
// Created by Bear on 2023/7/6.
//

#pragma once

#include "bbox.h"

namespace beart {
class AABB : public BBox {
 public:
  AABB() : p_min_(0.0), p_max_(kMaxFloat) {}
  AABB(Point3f p_min, Point3f p_max) : p_min_(std::move(p_min)), p_max_(std::move(p_max)) {}
  AABB(const AABB &aabb) : p_min_(aabb.p_min_), p_max_(aabb.p_max_) {}
  AABB(AABB &&other) : p_min_(std::move(other.p_min_)), p_max_(std::move(other.p_max_)) {}
  AABB &operator=(const AABB &other) {
    p_min_ = other.p_min_;
    p_max_ = other.p_max_;
    return *this;
  }
  AABB &operator=(AABB &&other) noexcept {
    p_min_ = std::move(other.p_min_);
    p_max_ = std::move(other.p_max_);
    return *this;
  }
  BBoxType GetBBoxType() const override {
    return BBoxType::kAABB;
  }
  const Point3f &operator[](uint i) const {
    return i == 0 ? p_min_ : p_max_;
  }
  Point3f &operator[](uint i) {
    return i == 0 ? p_min_ : p_max_;
  }

  float HalfSurfaceArea() const override;
  float SurfaceArea() const override;
  float Volume() const override;
  Point3f Centroid() const override;
  unsigned int MaxAxisExtents() const override;
  std::pair<const Point3f &, const Point3f &> Bounds() const {
    return std::make_pair(p_min_, p_max_);
  }
  bool IsInBox(const Point3f &p) const override;

  void Union(const AABB &other);
  void Union(const Point3f &p);
  bool Intersect(const Ray &ray) const override;

 private:
  Point3f p_min_;
  Point3f p_max_;
};
}
