//
// Created by Bear on 2021/12/3.
//

#pragma once

#include "light.h"
#include "primitive.h"
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
  void Prepare() {
    auto generateAABB = [](const std::vector<const Primitive *> &primitive) {
      AABB box;
      for (const auto &item : primitive) {
        box.Union(item->Bbox());
      }
      return box;
    };
    world_aabb_ = generateAABB(primitives_);
    accelerator_->Build(&primitives_, &world_aabb_);
  }

  [[nodiscard]] const Camera *camera() const {
    return camera_;
  }
  void set_camera(const Camera *camera) {
    camera_ = camera;
  }
  bool IntersectInfo(const Ray &ray, intersectionInfo *info) {
    return accelerator_->IntersectInfo(ray, info);
  }

 private:
  std::vector<const Light *> lights_;
  std::vector<const Primitive *> primitives_;
  const Camera *camera_ = nullptr;
  std::unique_ptr<Accelerator> accelerator_ = std::make_unique<BVH>();
  AABB world_aabb_;
};
}
