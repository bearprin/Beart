//
// Created by Bear on 2021/12/3.
//

#pragma once

#include "shape.h"
#include "bvh.h"
#include "camera.h"
#include "accelerator.h"

#include "common.h"
#include "aabb.h"
namespace beart {
class Scene {
 public:
  Scene() = default;
  void AddPrimitive(const Primitive *primitive) {
    this->primitives_.emplace_back(primitive);
  }
  void AddLight(const Light *light) {
    this->lights_.emplace_back(light);
  }
  [[nodiscard]] const Camera *camera() const {
    return camera_;
  }
  void set_camera(const Camera *camera) {
    camera_ = camera;
  }
  const std::vector<const Light *> lights() const {
    return lights_;
  }
  // TODO: Sample from multiple lights
  // For now just return the first light
  const Light *SampleLight(float u, float *pdf) const {
    *pdf = 1.;
    return this->lights_[0];
  }
  void Prepare() {
    auto generateAABB = [](const std::vector<const Primitive *> &primitive) {
      AABB box;
      for (const auto &item : primitive) {
        box.Union(item->bbox());
      }
      return box;
    };
    world_aabb_ = generateAABB(primitives_);
    accelerator_->Build(&primitives_, &world_aabb_);
  }

  bool IntersectInfo(const Ray &ray, IntersectionInfo *info) const {
    return accelerator_->IntersectInfo(ray, info);
  }
  [[nodiscard]] bool IsOccluded(const Ray &ray) const {
    IntersectionInfo info;
    info.query_shadow = true;
    return accelerator_->IntersectInfo(ray, &info);
  }
  Spectrum Le(const Ray &ray) const {
    if (!sky_light_) {
      return Spectrum{0.f, 0.f, 0.f};
    }
    // TODO: Implement sky light
//    return sky_light_->Le(ray);
    return Spectrum{0.f, 0.f, 0.f};
  }

 private:
  std::vector<const Light *> lights_;
  std::vector<const Primitive *> primitives_;
  const Camera *camera_ = nullptr;
  std::unique_ptr<Accelerator> accelerator_ = std::make_unique<BVH>();
  AABB world_aabb_;
  Light *sky_light_ = nullptr;
};
}
