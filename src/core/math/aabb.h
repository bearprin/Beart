//
// Created by Bear on 2021/12/8.
//

#pragma once

#include "vec.h"
#include "ray.h"

namespace beart {
class AABB {
 public:
  AABB();

  AABB(const Vec3f &p_0, const Vec3f &p_1);

  void Union(const AABB &rhs);

  void Union(const Vec3f &point);

  [[nodiscard]] unsigned int MaxAxisExtents() const;

  [[nodiscard]] float Delta(const unsigned int axis) const;

  [[nodiscard]] bool IsInBox(const Vec3f &p) const;

  [[nodiscard]] Vec3f Centroid() const;

  [[nodiscard]] float HalfSurfaceArea() const;

  [[nodiscard]] float SurfaceArea() const;

  [[nodiscard]] bool Intersect(const Ray &ray) const;

  [[nodiscard]] const Vec3f *bounds() const;

  void set_bounds(unsigned int min_max_id, unsigned int axis, float value);

 private:
  Vec3f bounds_[2];  // min_point: 0, max_point: 1
};

inline AABB Union(const AABB &lhs, const AABB &rhs) {
  AABB res;
  for (unsigned int i = 0; i < 3; ++i) {
    res.set_bounds(0, i, std::fmin(lhs.bounds()[0].data()[i], rhs.bounds()[0].data()[i]));
    res.set_bounds(1, i, std::fmax(lhs.bounds()[1].data()[i], rhs.bounds()[1].data()[i]));
  }
  return res;
};
}
