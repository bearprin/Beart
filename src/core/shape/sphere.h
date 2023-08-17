//
// Created by Bear on 2023/7/6.
//

#pragma once

#include <utility>

#include "vec.h"
#include "shape.h"
namespace beart {
class Sphere : public Shape {
 public:
  explicit Sphere(Vec3f center = Vec3f{0.f, 0.f, 0.f}, float radius = 0.5f)
      : radius_(radius), center_(std::move(center)) {
    obj_to_world_ = Translate(center_) * Scale(Vec3f{radius_});
    world_to_obj_ = Scale(Vec3f{1.f / radius_}) * Translate(-center_);
  }
  Sphere(const json &j) : Sphere(j.value("center", Vec3f{0.f, 0.f, 0.f}), j.value("radius", 1.0f)) {
//    if (j.contains("transform")) {
    obj_to_world_ = j.value("transform", Transform{1.f});
//      for (auto &t : obj_to_world_.matrix_) {
//        for (auto v : t) {
//          spdlog::info("Sphere transform: {}", v);
//        }
//      }
//    }
    world_to_obj_ = Inverse(obj_to_world_);
    center_ = obj_to_world_ * center_;
//    if (obj_to_world_.matrix_[0][0] != 0.f) {
      radius_ = obj_to_world_.matrix_[0][0] * radius_;
//    }
    spdlog::info("Sphere radius_: {}", radius_);

  }
  Point3f SampleDirect(const LightSample &ls,
                       const Point3f &inter_pos,
                       Vec3f &wi,
                       Vec3f *n,
                       float *pdf_solid) const override;
  void SampleDirect(const LightSample &ls_pos,
                    const LightSample &ls_dir,
                    Ray &ray,
                    Vec3f &n,
                    float *pdf_solid) const override;
  float DirectPdf(const Point3f &p, const Vec3f &wi) const override;
  bool Intersect(const Ray &ray) const override;
  bool Intersect(const Ray &ray, SurfaceInterection *inter) const override;
  float SurfaceArea() const override;
  float Volume() const override;
  const AABB &bbox() const override;

  const Vec3f &center() const {
    return center_;
  }
  float radius() const {
    return radius_;
  }
 private:
  float radius_;
  Vec3f center_;

};
}
