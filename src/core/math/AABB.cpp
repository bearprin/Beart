//
// Created by Bear on 2021/12/8.
//

#include "AABB.h"
#include "common.h"
beart::AABB::AABB() {
  bounds[0] =
      Vec3f{kMinFloat, kMinFloat, kMinFloat};
  bounds[1] =
      Vec3f{kMaxFloat, kMaxFloat, kMaxFloat};
}
beart::AABB::AABB(const beart::Vec3f &p_0, const beart::Vec3f &p_1) {
  for (unsigned int i = 0; i < 3; ++i) {
    auto[min_v, max_v] = std::minmax(p_0.data()[i], p_1.data()[i]);
    bounds[0].data()[i] = min_v;
    bounds[1].data()[i] = max_v;
  }
}
bool beart::AABB::IsInBox(const beart::Vec3f &p) const {
  for (unsigned int i = 0; i < 3; ++i) {
    if (!(p.data()[i] > bounds[0].data()[i] && p.data()[i] < bounds[1].data()[i])) {
      return false;
    }
  }
  return true;
}
void beart::AABB::Union(const beart::AABB &rhs) {
  for (unsigned int i = 0; i < 3; ++i) {
    bounds[0].data()[i] = std::fmin(bounds[0].data()[i], rhs.bounds[0].data()[i]);
    bounds[1].data()[i] = std::fmax(bounds[1].data()[i], rhs.bounds[1].data()[i]);
  }
}
// Williams et al. <<An Efﬁcient and Robust Ray-Box Intersection Algorithm>>
bool beart::AABB::Intersect(const beart::Ray &ray) const {
  float t_min{};
  float t_max{};
  float t_x_min{};
  float t_x_max{};
  float t_y_min{};
  float t_y_max{};
  float t_z_min{};
  float t_z_max{};

  Vec3f inv_dir = ray.inv_dir();
  // x case
  t_x_min = (bounds[ray.sign()[0]].x() - ray.ori().x()) * inv_dir.x();
  t_x_max = (bounds[1 - ray.sign()[0]].x() - ray.ori().x()) * inv_dir.x();
  // y case
  t_y_min = (bounds[ray.sign()[1]].y() - ray.ori().y()) * inv_dir.y();
  t_y_max = (bounds[1 - ray.sign()[1]].y() - ray.ori().y()) * inv_dir.y();

  // is valid?
  if (t_x_min > t_y_max || t_y_min > t_x_max) {
    return false;
  }
  // update max and min (more compact)
  t_min = std::fmax(t_x_min, t_y_min);
  t_max = std::fmin(t_x_max, t_y_max);

  t_z_min = (bounds[ray.sign()[2]].z() - ray.ori().z()) * inv_dir.z();
  t_z_max = (bounds[1 - ray.sign()[2]].z() - ray.ori().z()) * inv_dir.z();

  // is valid?
  if (t_min > t_z_max || t_z_min > t_max) {
    return false;
  }
  // update max and min (more compact)
  t_min = std::fmax(t_min, t_z_min);
  t_max = std::fmin(t_max, t_z_max);

  return t_min < ray.t_max() && t_max > ray.t_min();

}
float beart::AABB::HalfSurfaceArea() const {
  Vec3f offset = bounds[1] - bounds[0];
  return offset.x() * offset.y() + offset.y() * offset.z() + offset.z() * offset.x();
}
float beart::AABB::SurfaceArea() const {
  return HalfSurfaceArea() * 2;
}
beart::Vec3f beart::AABB::Centroid() const {
  return 0.5f * (bounds[0] + bounds[1]);
}

