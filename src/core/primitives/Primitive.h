//
// Created by Bear on 2021/12/3.
//

#ifndef BEART_SRC_CORE_PRIMITIVES_PRIMITIVE_H_
#define BEART_SRC_CORE_PRIMITIVES_PRIMITIVE_H_

#include "Ray.h"
#include "IntersectionInfo.h"
#include "Transform.h"

namespace beart {
class Primitive {
 public:
  virtual ~Primitive() {};
  virtual bool Intersect(const Ray &ray) const = 0;
  virtual bool IntersectInfo(const Ray &ray, std::shared_ptr<IntersectionInfo> info) const = 0;
 protected:
  Transform model_transform_;
};
}

#endif //BEART_SRC_CORE_PRIMITIVES_PRIMITIVE_H_
