//
// Created by Bear on 2023/7/6.
//

#include "aabb.h"
float beart::AABB::HalfSurfaceArea() const {
  Vec3f diagonal = p_max_ - p_min_;
  // (xy + yz + zx) to compute the half surface area
  return diagonal.x() * diagonal.y() + diagonal.y() * diagonal.z() + diagonal.z() * diagonal.x();
}
float beart::AABB::SurfaceArea() const {
  return 2.f * HalfSurfaceArea();
}
float beart::AABB::Volume() const {
  Vec3f diagonal = p_max_ - p_min_;
  // x * y * z to compute the volume
  return diagonal.x() * diagonal.y() * diagonal.z();
}
unsigned int beart::AABB::MaxAxisExtents() const {
  Vec3f diagonal = p_max_ - p_min_;
  if (diagonal.x() > diagonal.y() && diagonal.x() > diagonal.z()) {
    return 0;
  }
  if (diagonal.y() > diagonal.x() && diagonal.y() > diagonal.z()) {
    return 1;
  }
  return 2;
}
beart::Vec3f beart::AABB::Centroid() const {
  return (p_min_ + p_max_) * 0.5f;
}
void beart::AABB::Union(const beart::AABB &other) {
  p_min_ = Min(p_min_, other.p_min_);
  p_max_ = Max(p_max_, other.p_max_);
}
void beart::AABB::Union(const beart::Point3f &p) {
  p_min_ = Min(p_min_, p);
  p_max_ = Max(p_max_, p);
}

bool beart::AABB::IsInBox(const beart::Point3f &p) const {
  // judge point inside the aabb
  if (p.x() < p_min_.x() || p.x() > p_max_.x()) {
    return false;
  }
  if (p.y() < p_min_.y() || p.y() > p_max_.y()) {
    return false;
  }
  if (p.z() < p_min_.z() || p.z() > p_max_.z()) {
    return false;
  }
  return true;
}
bool beart::AABB::Intersect(const beart::Ray &ray) const {
  // <<An Efﬁcient and Robust Ray-Box Intersection Algorithm>>
  float t_min{};
  float t_max{};
  float t_ymin{};
  float t_ymax{};
  float t_zmin{};
  float t_zmax{};

  t_min = (this->operator[](ray.sign_[0]).x() - ray.ori_.x()) * ray.inv_dir_.x();
  t_max = (this->operator[](1 - ray.sign_[0]).x() - ray.ori_.x()) * ray.inv_dir_.x();
  t_ymin = (this->operator[](ray.sign_[1]).y() - ray.ori_.y()) * ray.inv_dir_.y();
  t_ymax = (this->operator[](1 - ray.sign_[1]).y() - ray.ori_.y()) * ray.inv_dir_.y();
  if ((t_min > t_ymax) || (t_ymin > t_max)) {
    return false;
  }
  if (t_ymin > t_min) {
    t_min = t_ymin;
  }
  if (t_ymax < t_max) {
    t_max = t_ymax;
  }
  t_zmin = (this->operator[](ray.sign_[2]).z() - ray.ori_.z()) * ray.inv_dir_.z();
  t_zmax = (this->operator[](1 - ray.sign_[2]).z() - ray.ori_.z()) * ray.inv_dir_.z();
  if ((t_min > t_zmax) || (t_zmin > t_max)) {
    return false;
  }
  if (t_zmin > t_min) {
    t_min = t_zmin;
  }
  if (t_zmax < t_max) {
    t_max = t_zmax;
  }
  return (t_min < ray.t_max_) && (t_max > ray.t_min_);
}
float beart::AABB::Delta(const unsigned int axis) const {
  return p_max_.data()[axis] - p_min_.data()[axis];
}
float beart::AABB::Intersect(const beart::AABB &lhs, const beart::Ray &ray) {
  float neat_t = kMinFloat;
  float far_t = kMaxFloat;

  for (int i = 0; i < 3; i++) {
    float origin = ray.ori_.data()[i];
    float min_val = lhs[0].data()[i];
    float max_val = lhs[1].data()[i];

    if (ray.dir_.data()[i] == 0) {
      if (origin < min_val || origin > max_val) {
        return false;
      }
    } else {
      float t1 = (min_val - origin) * ray.inv_dir_.data()[i];
      float t2 = (max_val - origin) * ray.inv_dir_.data()[i];

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
  if (ray.t_min_ <= far_t && neat_t <= ray.t_max_) {
    return neat_t < 0 ? far_t : neat_t;
  }
  return -1;
}
