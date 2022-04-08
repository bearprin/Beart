//
// Created by Bear on 2021/12/3.
//

#pragma once

#include "primitive.h"
#include "common.h"

namespace beart {
class Sphere : public Primitive {
 public:
  explicit Sphere(Vec3f ori = Vec3f{0.f, 0.f, 0.f}, float radius = 1.0f);
  const AABB &Bbox() const override;
  [[nodiscard]] bool Intersect(const Ray &ray) const override;
  [[nodiscard]] bool IntersectInfo(const Ray &ray, IntersectionInfo *) const override;
 private:
  Vec3f center_;
  float radius_;
  float radius_sq_;
};
}
