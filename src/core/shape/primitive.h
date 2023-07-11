//
// Created by Bear on 2023/7/09.
//

#pragma once

#include "common.h"
#include "shape.h"
#include "bxdf.h"

#include <vector>

namespace beart {
class Primitive {
 public:
  Primitive(const Shape *shape) : shape_(shape) {}
//  Primitive(const Shape *shape) : shape_(shape), bxdfs_({????}) {}
  Primitive(const Shape *shape, std::shared_ptr<Bxdf> bxdf) : shape_(shape), bxdfs_({bxdf}) {}

  bool Intersect(const Ray &ray) const {
    return shape_->Intersect(ray);
  }
  bool Intersect(const Ray &ray, SurfaceInterection *info) const {
    if (shape_->Intersect(ray, info)) {
      info->primitive = this;
      return true;
    }
    return false;
  }
  const AABB &bbox() const { return shape_->bbox(); }
  const Shape *shape() const { return shape_; }
  const std::vector<std::shared_ptr<Bxdf>> *bxdfs() const {
    return &bxdfs_;
  }
  std::shared_ptr<const Bxdf> bxdf(unsigned int idx) const {
    return bxdfs_[idx];
  }
  void set_bxdf(std::shared_ptr<Bxdf> bxdf) {
    bxdfs_.emplace_back(bxdf);
  }
  const Light *light() const { return light_; }
 private:
  const Shape *shape_ = nullptr;
  const Light *light_ = nullptr;  // primitive may emit light
  std::vector<std::shared_ptr<Bxdf>> bxdfs_;
};
}

