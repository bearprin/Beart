//
// Created by Bear on 2023/7/09.
//

#pragma once

#include "common.h"
#include "shape.h"
#include "bxdf.h"
#include "factory.h"
#include <vector>

namespace beart {
class Primitive {
 public:
  explicit Primitive(const Shape *shape) : shape_(shape) {}
  Primitive(const Shape *shape, const Light *light) : shape_(shape), light_(light) {}
  Primitive(const Shape *shape, const Light *light, std::shared_ptr<Bxdf> bxdf)
      : shape_(shape), light_(light), bxdfs_({bxdf}) {}
  Primitive(const Shape *shape, std::shared_ptr<Bxdf> bxdf) : shape_(shape), bxdfs_({bxdf}) {}

//  Primitive(const json &j) {
//    shape_ = BeartFactory<Shape>::Find(j, "shapes").get();
////    if (j.contains("light")) {
////      light_ = BeartFactory<Light>::CreateUnique(j["light"]).get();
////    }
//    if (j.contains("material")) {
//      for (auto &b : j["material"]) {
//        auto bxdf = BeartFactory<Bxdf>::Find(b);
//        bxdfs_.emplace_back(std::move(bxdf));
//      }
//    } else if (!light_) {
//      auto bxdf = BeartFactory<Bxdf>::CreateShared({{"type", "lambertian"}});
//      bxdfs_.emplace_back(std::move(bxdf));
//    }
//  }

  bool Intersect(const Ray &ray) const {
    return shape_->Intersect(ray);
  }
  bool Intersect(const Ray &ray, SurfaceInterection *info) const {
    if (shape_->Intersect(ray, info)) {
      info->primitive = this;
      // TODO: assuming one bxdf now
      info->albedo = bxdfs_.empty() ? Spectrum(1.f, 1.f, 1.f) : bxdfs_[0]->Albedo(-info->view, info->Ns);
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
  const Light *light() const { return light_; }
  void add_bxdf(std::shared_ptr<Bxdf> bxdf) {
    bxdfs_.emplace_back(bxdf);
  }
 private:
  const Shape *shape_ = nullptr;
  const Light *light_ = nullptr;  // primitive may emit light
  std::vector<std::shared_ptr<Bxdf>> bxdfs_;
};
}
//BEART_REGISTER_CLASS_IN_FACTORY(Primitive, Primitive, "surface")


