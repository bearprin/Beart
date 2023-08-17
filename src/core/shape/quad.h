//
// Created by Bear on 2023/7/28.
//

#pragma once

#include <utility>

#include "shape.h"
namespace beart {
class Quad : public Shape {
 public:
  Quad(float width = 2., float height = 2., Vec3f center = {0, 0, 0}, Vec3f normal = {0, 0, 1})
      : width_(width), height_(height), quad_center_(std::move(center)), quad_normal_(std::move(normal)) {
    obj_to_world_ = Translate(quad_center_) * Scale({width_ / 2, height_ / 2, 1.f});
    world_to_obj_ = Inverse(obj_to_world_);
  }
  Quad(float width, float height, const Transform &obj_to_world)
      : width_(width), height_(height) {
    obj_to_world_ = obj_to_world;
    world_to_obj_ = Inverse(obj_to_world_);
  }

  Quad(Vec3f center = {0, 0, 0}, Vec3f normal = {0, 0, 1})
      : quad_center_(std::move(center)), quad_normal_(std::move(normal)) {
    obj_to_world_ = Translate(quad_center_) * Scale({width_ / 2, height_ / 2, 1.f});
    world_to_obj_ = Inverse(obj_to_world_);
  }
  Quad(const json &j) : width_(j.value("width", 2.f)), height_(j.value("height", 2.f)) {
    if (j.contains("transform")) {
      obj_to_world_ = Transform(j["transform"]) * Scale({width_ / 2, height_ / 2, 1.f});
      world_to_obj_ = Inverse(obj_to_world_);
    } else {
      quad_center_ = j.value("center", Vec3f{0, 0, 0});
      quad_normal_ = j.value("normal", Vec3f{0, 0, 1});
      obj_to_world_ = Translate(quad_center_) * Scale({width_ / 2, height_ / 2, 1.f});
      world_to_obj_ = Inverse(obj_to_world_);
    }
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

 private:
  // assume quad is in xy plane [-1, 1]
  float width_ = 2.f;
  float height_ = 2.f;
  Vec3f quad_center_ = {0, 0, 0};
  Vec3f quad_normal_ = {0, 0, 1};
};
}
