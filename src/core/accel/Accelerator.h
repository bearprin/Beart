//
// Created by Bear on 2021/12/4.
//

#ifndef BEART_SRC_CORE_ACCEL_ACCELERATOR_H_
#define BEART_SRC_CORE_ACCEL_ACCELERATOR_H_

#include "Primitive.h"

namespace beart {

class Accelerator {
 public:
  virtual void Build(const std::vector<std::unique_ptr<const Primitive>> *primitives, const AABB *bbox) {
    primitives_ = primitives;
    world_aabb_ = bbox;
  };
  virtual bool IntersectInfo(const Ray &ray, IntersectionInfo *info) const = 0;
 protected:
  const std::vector<std::unique_ptr<const Primitive>> *primitives_ = nullptr;
  const AABB *world_aabb_ = nullptr;
};
}

#endif //BEART_SRC_CORE_ACCEL_ACCELERATOR_H_
