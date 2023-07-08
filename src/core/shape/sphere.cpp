//
// Created by Bear on 2023/7/6.
//

#include "sphere.h"
bool beart::Sphere::Intersect(const beart::Ray &ray) const {
  return Intersect(ray, nullptr);
}
bool beart::Sphere::Intersect(const beart::Ray &ray, beart::SurfaceInterection *inter) const {
  Vec3f L = ray.ori_ - this->center_;
  float a = Dot(ray.dir_, ray.dir_);
  float b = 2.f * Dot(ray.dir_, L);
  float c = Dot(L, L) - radius_ * radius_;
  auto [interect, t0, t1] = SolveQuadratic(a, b, c);
  if (!interect) {
    return false;
  }
  const auto limits = inter ? inter->t_curr : kMaxFloat;
  if (t0 > limits || t1 < 0.f) {
    return false;
  }
  if (t0 < ray.t_min_ && t1 > limits) {
    return false;
  }
  float t{};
  if (t0 > 0.f) {
    t = t0;
  } else if (t1 > limits) {
    return false;
  } else {
    t = t1;
  }
  if (t > ray.t_max_ || t < ray.t_min_) {
    return false;
  }
  if (inter) {
    inter->t_curr = t;
    inter->intersect_pos = ray(t);
    inter->view = -ray.dir_;
    inter->Ns = Normalize((inter->intersect_pos - this->center_));
    inter->Ng = inter->Ns;  // Ng eq Ns since this is an implicit equation
    Vec3f v1;
    Vec3f v2;
    CoordinateSystem(inter->Ns, &v1, &v2); // build local coordinate system with shading normal
    inter->tangent = v1;
  }
  return true;
}
const beart::AABB &beart::Sphere::AABBBounds() {
  if (!aabb_) {
    aabb_ = std::make_unique<AABB>(center_ - radius_, center_ + radius_);
  }
  return *aabb_;
}
float beart::Sphere::SurfaceArea() const {
  return 4 * kPi * radius_ * radius_;
}
