//
// Created by Bear on 2023/7/8.
//

#pragma once
#include "primitive.h"

#include <vector>
namespace beart {

class Accelerator {
 public:
  Accelerator() = default;
  virtual ~Accelerator() = default;
  virtual void Build(const std::vector<const Primitive *> *primitives, const AABB *bbox) {
    // Note: should task care the children shape of the primitives by accelerator
    primitives_ = primitives;
    world_aabb_ = bbox;
  };
  /// \brief Check if the ray is occluded by any primitive in the scene (sha
  /// \param ray
  /// \return
  virtual bool IsOccupied(const Ray &ray) const {
    // naive implementation loop for all primitive (just for debugging)
    return std::any_of(primitives_->cbegin(), primitives_->cend(), [&ray](const Primitive *primitive) {
      return primitive->Intersect(ray);
    });
  }
  virtual bool Intersect(const Ray &ray, SurfaceInterection *info) const {
    // naive implementation loop for all primitive (just for debugging)
    auto temp_r = ray;
    SurfaceInterection temp_info;
    for (const auto primitive : *primitives_) {
      if (primitive->Intersect(temp_r, &temp_info)) {
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