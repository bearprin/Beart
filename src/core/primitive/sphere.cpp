//
// Created by Bear on 2021/12/3.
//

#include "sphere.h"
#include "quadratic.h"
#include "intersection_info.h"
beart::Sphere::Sphere(beart::Vec3f center, float radius) : center_(std::move(center)), radius_(radius) {
  radius_sq_ = radius_ * radius_;
}
bool beart::Sphere::Intersect(const beart::Ray &ray) const {
  Vec3f L = ray.dir() - this->center_;
  float a = Dot(ray.dir(), ray.dir());
  float b = 2.f * Dot(ray.dir(), L);
  float c = Dot(L, L) - radius_sq_;
  auto[interect, t0, t1] = SolveQuadratic(a, b, c);
  return interect;
}
bool beart::Sphere::IntersectInfo(const beart::Ray &ray, IntersectionInfo *info) const {
  Vec3f L = ray.ori() - this->center_;
  float a = Dot(ray.dir(), ray.dir());
  float b = 2.f * Dot(ray.dir(), L);
  float c = Dot(L, L) - radius_sq_;
  auto[interect, t0, t1] = SolveQuadratic(a, b, c);
  if (!interect) {
    return false;
  }
  const auto limits = info ? info->t_curr : kMaxFloat;
  if (t0 > limits || t1 < 0.f) {
    return false;
  }
  float t;
  if (t0 > 0.0f) {
    t = t0;
  } else if (t1 > limits) {
    return false;
  } else {
    t = t1;
  }

  if (t > ray.t_max() || t < ray.t_min()) {
    return false;
  }
  if (info) {
    info->happened = true;
    info->t_curr = t;
    info->corrds = ray(t);
    info->view = -ray.dir();
    info->Ns = Normalize((info->corrds - this->center_));
    info->Ng = info->Ns;  // Ng eq Ns since this is an implicit equation
    Vec3f v1;
    Vec3f v2;
    CoordinateSystem(info->Ns, &v1, &v2); // build local coordinate system with shading normal
    info->tangent = v1;
  }
  return interect;
}
const beart::AABB &beart::Sphere::bbox() const {
  if (!bbox_ptr) {
    bbox_ptr = std::make_unique<beart::AABB>();
    auto vec_r = Vec3f{radius_, radius_, radius_};
    bbox_ptr->set_bounds(0, center_ - vec_r);
    bbox_ptr->set_bounds(1, center_ + vec_r);
  }
  return *bbox_ptr;
}
