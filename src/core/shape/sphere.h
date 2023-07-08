//
// Created by Bear on 2023/7/6.
//

#pragma once

#include "vec.h"
#include "shape.h"
namespace beart {
class Sphere : public Shape {
 public:
  explicit Sphere(Vec3f center = Vec3f{0.f, 0.f, 0.f}, float radius = 0.5f)
      : radius_(radius), center_(center) {
    obj_to_world_ = Translate(center_) * Scale(Vec3f{radius_});
    world_to_obj_ = Scale(Vec3f{1.f / radius_}) * Translate(-center_);
  }
  bool Intersect(const Ray &ray) const override;
  bool Intersect(const Ray &ray, SurfaceInterection *inter) const override;
  float SurfaceArea() const override;
  float Volume() const override;
  const AABB& bbox() const override;
 private:
  float radius_;
  Vec3f center_;

};
}
