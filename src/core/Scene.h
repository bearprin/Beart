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
namespace beart {
class Scene {
 public:
  Scene() = default;
  void AddPrimitive(const std::shared_ptr<Primitive> &primitive) {
    this->primitives_.emplace_back(primitive);
  }
  void AddLight(const std::shared_ptr<Light> &light) {
    this->lights_.emplace_back(light);
  }

 private:
  std::vector<std::shared_ptr<Light>> lights_;
  std::vector<std::shared_ptr<Primitive>> primitives_;
  std::shared_ptr<Camera> camera_;
  std::shared_ptr<Accelerator> accelerator_;

};
}
#endif //BEART_SRC_CORE_SCENE_H_
