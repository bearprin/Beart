//
// Created by Bear on 2021/12/3.
//

#pragma once

#include "light.h"
#include "primitive.h"
#include "camera.h"
#include "accelerator.h"

#include "common.h"
#include "aabb.h"
namespace beart {
class Scene {
 public:
  Scene() = default;
  void AddPrimitive(Primitive *primitive) {
    this->primitives_.emplace_back(primitive);
  }
  void AddLight(Light *light) {
    this->lights_.emplace_back(light);
  }
  void Prepare() {
    auto generateAABB = [](const std::vector<const Primitive *> &primitive) {
      AABB box;
      for (const auto &item : primitive) {
        box.Union(item->GetBBox());
      }
      return box;
    };
    world_aabb_ = generateAABB(primitives_);
  }
  void BuildAccelerationStructure() {
    accelerator_->Build(&primitives_, &world_aabb_);
  }

 private:
  std::vector<const Light *> lights_;
  std::vector<const Primitive *> primitives_;
  std::unique_ptr<Camera> camera_;
  std::unique_ptr<Accelerator> accelerator_;
  AABB world_aabb_;

};
}
