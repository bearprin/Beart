//
// Created by Bear on 2023/07/03.
//

#pragma once

#include "json_serializable.h"
#include "accelerator.h"
#include "common.h"
#include "camera.h"
namespace beart {
class Scene : public JsonSerializable {
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
  const std::vector<const Primitive *> &primitives() const {
    return primitives_;
  }
  const Light *SampleLight(float u, float *pdf) const {
    *pdf = 1.;
    // TODO: Sample from multiple lights. For now just return the first light
    return this->lights_[0];
  }
  void Prepare() {
    AABB world_aabb;
    for (const auto item : primitives_) {
      world_aabb.Union(item->bbox());
    }
    world_aabb_ = world_aabb;
    accelerator_->Build(&primitives_, &world_aabb_);
  }

  bool Intersect(const Ray &ray, SurfaceInterection *info) const {
    return accelerator_->Intersect(ray, info);
  }
  bool IsOccluded(const Ray &ray) const {
    return accelerator_->IsOccupied(ray);
  }
  Spectrum Le(const Ray &ray) const {
    if (!sky_light_) {
      return Spectrum{0.f, 0.f, 0.f};
    }
//     TODO: Implement sky light (how to sample sky light from env map?)
//    return sky_light_->Le(ray);
    return Spectrum{0.f, 0.f, 0.f};
  }

 private:
  std::vector<const Light *> lights_;
  std::vector<const Primitive *> primitives_;
  const Camera *camera_ = nullptr;
  std::unique_ptr<Accelerator> accelerator_ = std::make_unique<Accelerator>();
  AABB world_aabb_;
  Light *sky_light_ = nullptr;
};
class Visibility {
 public:
  explicit Visibility(const Scene &scene) : scene_(scene) {}
  bool IsVisible() const {
    return !scene_.IsOccluded(ray_);
  }
  Ray ray_;
 private:
  const Scene &scene_;
};
}
