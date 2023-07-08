//
// Created by Bear on 2023/7/8.
//

#pragma once
#include "primitive.h"

namespace beart {

class Accelerator {
 public:
  Accelerator() = default;
  virtual ~Accelerator() = default;
  virtual void Build(const std::vector<const Primitive *> *primitives, const AABB *bbox) {
    primitives_ = primitives;
    world_aabb_ = bbox;
  };
  virtual bool Intersect(const Ray &ray, SurfaceInterection *info) const {
    // naive implementation loop for all primitive (just for debugging)
    auto temp_r = ray;
    SurfaceInterection temp_info;
    for (const auto primitive : *primitives_) {
      if (primitive->IntersectInfo(temp_r, &temp_info)) {
        temp_r.t_max_ = temp_info.t_curr;
        *info = temp_info;
      }
    }
    return info->primitive != nullptr;
  }

 protected:
  const std::vector<const Primitive *> *primitives_ = nullptr;
  const AABB *world_aabb_ = nullptr;
  bool is_valid_ = false; // use this to check if the accelerator is built
};
}