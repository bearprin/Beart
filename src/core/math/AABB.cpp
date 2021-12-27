//
// Created by Bear on 2021/12/8.
//

#include "AABB.h"

#include "common.h"
beart::AABB::AABB() {
  max_point_ =
      Vec3f{kMaxFloat, kMaxFloat, kMaxFloat};
  min_point_ =
      Vec3f{kMinFloat, kMinFloat, kMinFloat};
}
beart::AABB::AABB(const beart::Vec3f &p_0, const beart::Vec3f &p_1) {
  auto[min_x, max_x] = std::minmax(p_0.x(), p_1.x());
  min_point_.x() = min_x;
  max_point_.x() = max_x;

  auto[min_y, max_y] = std::minmax(p_0.y(), p_1.y());
  min_point_.y() = min_y;
  max_point_.y() = max_y;

  auto[min_z, max_z] = std::minmax(p_0.z(), p_1.z());
  min_point_.z() = min_z;
  max_point_.z() = max_z;
}
bool beart::AABB::IsInBox(const beart::Vec3f &p) const {
  if (p.x() > min_point_.x() && p.x() < max_point_.x()) {
    if (p.y() > min_point_.y() && p.y() < max_point_.y()) {
      if (p.z() > min_point_.z() && p.z() < max_point_.z()) {
        return true;
      }
    }
  }
  return false;
}
void beart::AABB::Union(const beart::AABB &rhs) {
  max_point_.x() = std::max(this->max_point_.x(), rhs.max_point_.x());
  max_point_.y() = std::max(this->max_point_.y(), rhs.max_point_.y());
  max_point_.z() = std::max(this->max_point_.z(), rhs.max_point_.z());

  min_point_.x() = std::min(this->min_point_.x(), rhs.min_point_.x());
  min_point_.y() = std::min(this->min_point_.y(), rhs.min_point_.y());
  min_point_.z() = std::min(this->min_point_.z(), rhs.min_point_.z());
}
beart::Vec3f beart::AABB::Diagonal() const {
  return max_point_ - min_point_;
}
bool beart::AABB::Intersect(const beart::Ray &ray) const {
  float t_x_min = 0;
  float t_x_max = 0;
  float t_y_min = 0;
  float t_y_max = 0;
  float t_z_min = 0;
  float t_z_max = 0;

  Vec3f inv_dir = Vec3f{1.f / ray.dir_.x(), 1.f / ray.dir_.y(), 1.f / ray.dir_.z()};
  // x case
  if (inv_dir.x() >= 0) {
    t_x_min = (min_point_.x() - ray.ori_.x()) * inv_dir.x();
    t_x_max = (max_point_.x() - ray.ori_.x()) * inv_dir.x();
  } else {
    t_x_min = (max_point_.x() - ray.ori_.x()) * inv_dir.x();
    t_x_max = (min_point_.x() - ray.ori_.x()) * inv_dir.x();
  }
  // y case
  if (inv_dir.y() >= 0) {
    t_y_min = (min_point_.y() - ray.ori_.y()) * inv_dir.y();
    t_y_max = (max_point_.y() - ray.ori_.y()) * inv_dir.y();
  } else {
    t_y_min = (max_point_.y() - ray.ori_.y()) * inv_dir.y();
    t_y_max = (min_point_.y() - ray.ori_.y()) * inv_dir.y();
  }
  // z case
  if (inv_dir.z() >= 0) {
    t_z_min = (min_point_.z() - ray.ori_.z()) * inv_dir.z();
    t_z_max = (max_point_.z() - ray.ori_.z()) * inv_dir.z();
  } else {
    t_z_min = (max_point_.z() - ray.ori_.z()) * inv_dir.z();
    t_z_max = (min_point_.z() - ray.ori_.z()) * inv_dir.z();
  }

  // is overlap with the valid range of three axis?
  return !(t_x_min > t_y_max || t_x_min > t_z_max || t_y_min > t_x_max || t_y_min > t_z_max || t_z_min > t_x_max
      || t_z_min > t_y_max);
}

