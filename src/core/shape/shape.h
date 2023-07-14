//
// Created by Bear on 2023/7/6.
//

#pragma once

#include "ray.h"
#include "aabb.h"
#include "transform.h"
#include "interection.h"
#include "json_serializable.h"

#include <memory>
#include <vector>
namespace beart {
class Shape : public JsonSerializable {
 public:
  virtual ~Shape() = default;
  virtual bool Intersect(const Ray &ray) const = 0;
  virtual bool Intersect(const Ray &ray, SurfaceInterection *inter) const = 0;
  virtual float SurfaceArea() const = 0;
  virtual float Volume() const = 0;
  virtual const AABB &bbox() const = 0;
  virtual void add_children(std::shared_ptr<Shape> child) {
    children_.emplace_back(child);
  }
  std::vector<std::shared_ptr<Shape>> children_;
  bool children_bvh_flag_ = false;
 protected:
  Transform obj_to_world_ = Transform{Mat4f{1.0}};  // from object space to world space, default is identity
  Transform world_to_obj_ = Transform{Mat4f{1.0}};  // from object space to world space, default is identity
  mutable std::unique_ptr<AABB> bbox_;
};
}