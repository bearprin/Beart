//
// Created by Bear on 2023/7/6.
//

#include "sphere.h"
bool beart::Sphere::Intersect(const beart::Ray &ray) const {
  return Intersect(ray, nullptr);
}
bool beart::Sphere::Intersect(const beart::Ray &ray, beart::SurfaceInterection *inter) const {
  const auto r = world_to_obj_ * ray; // transform ray from world space to object space
  const auto a = Dot(r.dir_, r.dir_);
  const auto b = 2.0f * Dot(r.dir_, r.ori_);
  const auto c = sqrt(r.ori_.x()) + sqrt(r.ori_.y()) + sqrt(r.ori_.z()) - radius_;

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
  const auto p = r(t);
  if (inter) {
    inter->t_curr = t;
    Vec3f n = Normalize(Vec3f{p.x(), p.y(), p.z()});
    Vec3f v0;
    Vec3f v1;
    CoordinateSystem(n, &v0, &v1); // build local coordinate system with shading normal
    inter->intersect = obj_to_world_.TransformPoint(p);
    inter->Ns = obj_to_world_.TransformNormal(n);
    inter->Ng = inter->Ns;
    inter->tangent = obj_to_world_.TransformVector(v0);
    inter->view = -ray.dir_;  // world space
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
