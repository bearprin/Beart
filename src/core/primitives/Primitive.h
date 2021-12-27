//
// Created by Bear on 2021/12/3.
//

#ifndef BEART_SRC_CORE_PRIMITIVES_PRIMITIVE_H_
#define BEART_SRC_CORE_PRIMITIVES_PRIMITIVE_H_

#include "Ray.h"
#include "IntersectionInfo.h"
#include "Transform.h"
#include "AABB.h"

namespace beart {
class Primitive {
 public:
  virtual ~Primitive() = default;
  [[nodiscard]] const AABB &GetBBox() const {
    return bbox_;
  }
  virtual bool Intersect(const Ray &ray) const = 0;
  [[nodiscard]] virtual bool IntersectInfo(const Ray &ray, IntersectionInfo *info) const = 0;
 protected:
  Transform model_transform_;
  AABB bbox_;
};
}

#endif //BEART_SRC_CORE_PRIMITIVES_PRIMITIVE_H_
