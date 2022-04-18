//
// Created by Bear on 2022/4/12.
//

#pragma once

#include "common.h"
#include "shape.h"
#include "material.h"
#include "intersection_info.h"
#include "bxdf.h"
#include "normal.h"
namespace beart {
class Primitive {
 public:
  explicit Primitive(const Shape *shape) : shape_(shape) {}
  Primitive(const Shape *shape, Bxdf *bxdf) : shape_(shape), bxdf_(bxdf) {}
  Primitive(const Shape *shape, const Light *light, Bxdf *bxdf) : shape_(shape), light_(light), bxdf_(bxdf) {}

  [[nodiscard]] bool Intersect(const Ray &ray) const {
    return shape_->Intersect(ray);
  }
  [[nodiscard]] bool IntersectInfo(const Ray &ray, IntersectionInfo *info) const {
    if (shape_->IntersectInfo(ray, info)) {
      info->primitive_ = this;
      return true;
    }
    return false;
  }
  [[nodiscard]] const AABB &bbox() const { return shape_->bbox(); }
  [[nodiscard]] const Shape *shape() const { return shape_; }
  [[nodiscard]] const Light *light() const { return light_; }
  [[nodiscard]]  Bxdf *material() const {
    return bxdf_;
  }
 private:
  const Shape *shape_ = nullptr;
  const Light *light_ = nullptr;
  Bxdf *bxdf_ = nullptr;
  unsigned int bxdfs_count_ = 0;
};
}

