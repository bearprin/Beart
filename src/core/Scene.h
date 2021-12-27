//
// Created by Bear on 2021/12/3.
//

#ifndef BEART_SRC_CORE_SCENE_H_
#define BEART_SRC_CORE_SCENE_H_

#include "Light.h"
#include "Primitive.h"
#include "Camera.h"
#include "Accelerator.h"

#include "common.h"
#include "AABB.h"
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
    auto generateAABB = [](const std::vector<std::unique_ptr<const Primitive>> &primitive) {
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
  std::vector<std::unique_ptr<const Light>> lights_;
  std::vector<std::unique_ptr<const Primitive>> primitives_;
  std::unique_ptr<Camera> camera_;
  std::unique_ptr<Accelerator> accelerator_;
  AABB world_aabb_;

};
}
#endif //BEART_SRC_CORE_SCENE_H_
