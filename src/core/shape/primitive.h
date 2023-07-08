//
// Created by Bear on 2023/7/09.
//

#pragma once

#include "common.h"
#include "shape.h"
#include "light.h"
//#include "material.h"
//#include "intersection_info.h"
//#include "bxdf.h"
//#include "normal.h"
namespace beart {
class Primitive {
 public:
  Primitive(const Shape *shape) : shape_(shape) {}
//  Primitive(const Shape *shape, Bxdf *bxdf) : shape_(shape), bxdf_(bxdf) {}
//  Primitive(const Shape *shape, const Light *light, Bxdf *bxdf) : shape_(shape), light_(light), bxdf_(bxdf) {}

  bool Intersect(const Ray &ray) const {
    return shape_->Intersect(ray);
  }
  bool IntersectInfo(const Ray &ray, SurfaceInterection *info) const {
    if (shape_->Intersect(ray, info)) {
      info->primitive = this;
      return true;
    }
    return false;
  }
  const AABB &bbox() const { return shape_->bbox(); }
  const Shape *shape() const { return shape_; }
  const Light *light() const { return light_; }
//  Bxdf *material() const {
//    return bxdf_;
//  }
 private:
  const Shape *shape_ = nullptr;
  const Light *light_ = nullptr;
//  Bxdf *bxdf_ = nullptr;  // TODO: How to handle multiple bxdfs?
  unsigned int bxdfs_count_ = 0;
};
}

