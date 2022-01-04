//
// Created by Bear on 2021/12/8.
//

#include "aabb.h"
#include "common.h"
beart::AABB::AABB() {
  bounds_[0] =
      Vec3f{kMaxFloat, kMaxFloat, kMaxFloat};
  bounds_[1] =
      Vec3f{kMinFloat, kMinFloat, kMinFloat};
}
beart::AABB::AABB(const beart::Vec3f &p_0, const beart::Vec3f &p_1) {
  for (unsigned int i = 0; i < 3; ++i) {
    auto[min_v, max_v] = std::minmax(p_0.data()[i], p_1.data()[i]);
    bounds_[0].data()[i] = min_v;
    bounds_[1].data()[i] = max_v;
  }
}
bool beart::AABB::IsInBox(const beart::Vec3f &p) const {
  for (unsigned int i = 0; i < 3; ++i) {
    if (!(p.data()[i] > bounds_[0].data()[i] && p.data()[i] < bounds_[1].data()[i])) {
      return false;
    }
  }
  return true;
}
void beart::AABB::Union(const beart::AABB &rhs) {
  for (unsigned int i = 0; i < 3; ++i) {
    bounds_[0].data()[i] = std::fmin(bounds_[0].data()[i], rhs.bounds_[0].data()[i]);
    bounds_[1].data()[i] = std::fmax(bounds_[1].data()[i], rhs.bounds_[1].data()[i]);
  }
}
void beart::AABB::Union(const beart::Vec3f &point) {
  for (auto i = 0u; i < 3; ++i) {
    bounds_[0].data()[i] = std::fmin(bounds_[0].data()[i], point.data()[i]);
    bounds_[1].data()[i] = std::fmax(bounds_[1].data()[i], point.data()[i]);
  }
}
// Williams et al. <<An Efficient and Robust Ray-Box Intersection Algorithm>>
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
  t_x_min = (bounds_[ray.sign()[0]].x() - ray.ori().x()) * inv_dir.x();
  t_x_max = (bounds_[1 - ray.sign()[0]].x() - ray.ori().x()) * inv_dir.x();
  // y case
  t_y_min = (bounds_[ray.sign()[1]].y() - ray.ori().y()) * inv_dir.y();
  t_y_max = (bounds_[1 - ray.sign()[1]].y() - ray.ori().y()) * inv_dir.y();

  // is valid?
  if (t_x_min > t_y_max || t_y_min > t_x_max) {
    return false;
  }
  // update max and min (more compact)
  t_min = std::fmax(t_x_min, t_y_min);
  t_max = std::fmin(t_x_max, t_y_max);

  t_z_min = (bounds_[ray.sign()[2]].z() - ray.ori().z()) * inv_dir.z();
  t_z_max = (bounds_[1 - ray.sign()[2]].z() - ray.ori().z()) * inv_dir.z();

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
  Vec3f offset = bounds_[1] - bounds_[0];
  return offset.x() * offset.y() + offset.y() * offset.z() + offset.z() * offset.x();
}
float beart::AABB::SurfaceArea() const {
  return HalfSurfaceArea() * 2;
}
beart::Vec3f beart::AABB::Centroid() const {
  return 0.5f * (bounds_[0] + bounds_[1]);
}
unsigned int beart::AABB::MaxAxisExtents() const {
  Vec3f offset = bounds_[1] - bounds_[0];
  if (offset.x() > offset.y() && offset.x() > offset.z())
    return 0;
  if (offset.y() > offset.x() && offset.y() > offset.z())
    return 1;

  return 2;
}
float beart::AABB::Delta(const unsigned int axis) const {
  return bounds_[1][axis] - bounds_[0][axis];
}
const beart::Vec3f *beart::AABB::bounds() const {
  return bounds_;
}
void beart::AABB::set_bounds(const unsigned int min_max_id, const unsigned int axis, const float value) {
  bounds_[min_max_id][axis] = value;
}
void beart::AABB::set_bounds(unsigned int min_max_id, const beart::Vec3f &p) {
  bounds_[min_max_id] = p;
}
