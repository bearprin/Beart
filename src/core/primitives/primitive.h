//
// Created by Bear on 2021/12/3.
//

#pragma once

#include "ray.h"
#include "intersectionInfo.h"
#include "transform.h"
#include "aabb.h"

namespace beart {
class Primitive {
 public:
  virtual ~Primitive() = default;
  [[nodiscard]] const AABB &GetBBox() const {
    return bbox_;
  }
  virtual bool Intersect(const Ray &ray) const = 0;
  [[nodiscard]] virtual bool IntersectInfo(const Ray &ray, intersectionInfo *info) const = 0;
 protected:
  Transform model_transform_;
  AABB bbox_;
};
}

