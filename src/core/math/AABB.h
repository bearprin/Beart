//
// Created by Bear on 2021/12/8.
//

#ifndef BEART_SRC_CORE_MATH_AABB_H_
#define BEART_SRC_CORE_MATH_AABB_H_

#include "Vec.h"
#include "Ray.h"
namespace beart {
class AABB {
 public:
  AABB();
  AABB(const Vec3f &p_0, const Vec3f &p_1);

  void Union(const AABB &rhs);

  [[nodiscard]] bool IsInBox(const Vec3f &p) const;

  [[nodiscard]] Vec3f Diagonal() const;

  [[nodiscard]] bool Intersect(const Ray &ray) const;

 private:
  Vec3f max_point_;
  Vec3f min_point_;
};
}

#endif //BEART_SRC_CORE_MATH_AABB_H_
