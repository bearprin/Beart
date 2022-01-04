//
// Created by Bear on 2021/12/4.
//

#pragma once

#include "primitive.h"

namespace beart {

class Accelerator {
 public:
  Accelerator() = default;
  virtual void Build(const std::vector<const Primitive *> *primitives, const AABB *bbox) {
    primitives_ = primitives;
    world_aabb_ = bbox;
  };
  virtual bool IntersectInfo(const Ray &ray, intersectionInfo *info) const = 0;
 protected:
  const std::vector<const Primitive *> *primitives_ = nullptr;
  const AABB *world_aabb_ = nullptr;
  bool is_valid_ = false;
};
}