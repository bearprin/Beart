//
// Created by Bear on 2021/12/3.
//

#pragma once

#include "ray.h"
#include "transform.h"
#include "aabb.h"
namespace beart {
class IntersectionInfo;
class Shape {
 public:
  virtual ~Shape() = default;
  virtual const AABB &bbox() const = 0;
  virtual bool Intersect(const Ray &ray) const = 0;
  [[nodiscard]] virtual bool IntersectInfo(const Ray &ray, IntersectionInfo *info) const = 0;
 protected:
  Transform model_transform_;
  mutable std::unique_ptr<AABB> bbox_ptr;
};
}

