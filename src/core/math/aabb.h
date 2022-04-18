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
  float t_min = 0;
  float t_max = 0;
  float t_x_min = 0;
  float t_x_max = 0;
  float t_y_min = 0;
  float t_y_max = 0;
  float t_z_min = 0;
  float t_z_max = 0;

  Vec3f inv_dir = ray.inv_dir();
  // x case
  t_x_min = (lhs.bounds()[ray.sign()[0]].x() - ray.ori().x()) * inv_dir.x();
  t_x_max = (lhs.bounds()[1 - ray.sign()[0]].x() - ray.ori().x()) * inv_dir.x();

  t_y_min = (lhs.bounds()[ray.sign()[1]].y() - ray.ori().y()) * inv_dir.y();
  t_y_max = (lhs.bounds()[1 - ray.sign()[1]].y() - ray.ori().y()) * inv_dir.y();

  if (t_x_min > t_y_max || t_y_min > t_x_max) {
    return -1;
  }
  // update max and min (more compact)
  t_min = std::fmax(t_x_min, t_y_min);
  t_max = std::fmin(t_x_max, t_y_max);

  t_z_min = (lhs.bounds()[ray.sign()[2]].z() - ray.ori().z()) * inv_dir.z();
  t_z_max = (lhs.bounds()[1 - ray.sign()[2]].z() - ray.ori().z()) * inv_dir.z();

  // is valid?
  if (t_min > t_z_max || t_z_min > t_max) {
    return -1;
  }

  t_min = std::fmax(t_min, t_z_min);
  t_max = std::fmin(t_max, t_z_max);

  if (t_min > ray.t_min() && t_max < ray.t_max() && t_min < t_max) {
    return t_min;
  }
  return -1;
}
}
