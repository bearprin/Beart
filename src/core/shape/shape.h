//
// Created by Bear on 2023/7/6.
//

#pragma once

#include "ray.h"
#include "aabb.h"
#include "transform.h"
#include "interection.h"
#include "json_serializable.h"
namespace beart {
class Shape : public JsonSerializable {
 public:
  virtual ~Shape() {}
  virtual bool Intersect(const Ray &ray) const = 0;
  virtual bool Intersect(const Ray &ray, SurfaceInterection *inter) const = 0;
  virtual float SurfaceArea() const = 0;
  virtual float Volume() const = 0;
  virtual const AABB &bbox() const = 0;
 protected:
  Transform obj_to_world_ = Transform{Mat4f{1.0}};  // from object space to world space, default is identity
  Transform world_to_obj_ = Transform{Mat4f{1.0}};  // from object space to world space, default is identity
  mutable std::unique_ptr<AABB> bbox_;
};
}