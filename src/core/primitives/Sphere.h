//
// Created by Bear on 2021/12/3.
//

#ifndef BEART_SRC_CORE_PRIMITIVES_SPHERE_H_
#define BEART_SRC_CORE_PRIMITIVES_SPHERE_H_

#include "Primitive.h"
#include "common.h"

namespace beart {
class Sphere : public Primitive {
 public:
  explicit Sphere(Vec3f ori = Vec3f{0.f, 0.f, 0.f}, float radius = 1.0f);
  [[nodiscard]] bool Intersect(const Ray &ray) const override;
  [[nodiscard]] bool IntersectInfo(const Ray &ray, IntersectionInfo *) const override;
 private:
  Vec3f center_;
  float radius_;
  float radius_sq_;
};
}

#endif //BEART_SRC_CORE_PRIMITIVES_SPHERE_H_
