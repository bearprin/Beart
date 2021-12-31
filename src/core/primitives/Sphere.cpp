//
// Created by Bear on 2021/12/3.
//

#include "Sphere.h"

#include "Quadratic.h"

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
  auto[interect, t_0, t_1] = SolveQuadratic(a, b, c);
  if (!interect) {
    return false;
  }
  if (t_0 < 0) {
    t_0 = t_1;
  }
  if (t_0 < 0) {
    return false;
  }

  info->happened = true;
  info->t_curr = t_0;
  info->corrds = ray(t_0);
  info->Ns = (info->corrds - this->center_).normalized();
  info->Ng = info->Ns;  // Ng eq Ns since this is an implicit equation
  info->obj = static_cast<const Primitive *>(this);
  return interect;
}
