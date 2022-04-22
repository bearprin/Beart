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

  void set_bounds(unsigned int min_max_id, const Vec3f &p);

 private:
  Vec3f bounds_[2];  // min_point: 0, max_point: 1
};

static AABB Union(const AABB &lhs, const AABB &rhs) {
  AABB res;
  for (unsigned int i = 0; i < 3; ++i) {
    res.set_bounds(0, i, std::fmin(lhs.bounds()[0][i], rhs.bounds()[0][i]));
    res.set_bounds(1, i, std::fmax(lhs.bounds()[1][i], rhs.bounds()[1][i]));
  }
  return res;
}
static float Intersect(const AABB &lhs, const Ray &ray) {
  float neat_t = kMinFloat;
  float far_t = kMaxFloat;

  for (int i = 0; i < 3; i++) {
    float origin = ray.ori().data()[i];
    float min_val = lhs.bounds()[0].data()[i];
    float max_val = lhs.bounds()[1].data()[i];

    if (ray.dir().data()[i] == 0) {
      if (origin < min_val || origin > max_val) {
        return false;
      }
    } else {
      float t1 = (min_val - origin) * ray.inv_dir().data()[i];
      float t2 = (max_val - origin) * ray.inv_dir().data()[i];

      if (t1 > t2) {
        std::swap(t1, t2);
      }
      neat_t = std::max(t1, neat_t);
      far_t = std::min(t2, far_t);
      if (neat_t > far_t) {
        return -1;
      }
    }
  }
  if (ray.t_min() <= far_t && neat_t <= ray.t_max()) {
    return neat_t < 0 ? far_t : neat_t;
  }
  return -1;
}
}
