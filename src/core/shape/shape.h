//
// Created by Bear on 2023/7/6.
//

#pragma once

#include "ray.h"
#include "aabb.h"
#include "transform.h"
#include "interection.h"
#include "json_serializable.h"
#include "samples.h"

#include <memory>
#include <vector>
namespace beart {
class Shape : public JsonSerializable {
 public:
  Shape() = default;
  explicit Shape(Transform obj_to_world) : obj_to_world_(std::move(obj_to_world)) {
    world_to_obj_ = Inverse(obj_to_world_);
  }
  virtual ~Shape() = default;
  /// given intersection, sample point on the surface of the shape
  /// \param ls
  /// \param inter_pos
  /// \param wi  (required, set as reference)
  /// \param n
  /// \param pdf_solid
  /// \return
  virtual Point3f SampleDirect(const LightSample &ls,
                               const Point3f &inter_pos,
                               Vec3f &wi,
                               Vec3f *n,
                               float *pdf_solid) const = 0;

  // Given pdf of sampled point and direction
  /// \param p  intersection point
  /// \param wi  direction from p to the sampled point
  /// \return
  virtual float DirectPdf(const Point3f &p, const Vec3f &wi) const {
    SurfaceInterection inter;
    if (!Intersect(Ray{p, wi, 1, false}, &inter)) {
      return 0.f;
    }
    Vec3f delta = Normalize(p - inter.intersect_pos);
    if (float dot = SafeDot(inter.Ng, delta); dot > 0.f) {
      float dist = Norm(delta);
      return dist * dist / (dot * SurfaceArea()); // convert pdf_a to pdf_s (d^2 / cos(theta) * (1 / area))
    }
    return 0.f;
  }
  virtual bool Intersect(const Ray &ray) const = 0;
  virtual bool Intersect(const Ray &ray, SurfaceInterection *inter) const = 0;
  virtual float SurfaceArea() const = 0;
  virtual float Volume() const = 0;
  virtual const AABB &bbox() const = 0;
  const Transform &ObjToWorld() const {
    return obj_to_world_;
  }
  const Transform &WorldToObj() const {
    return world_to_obj_;
  }
 protected:
  Transform obj_to_world_ = Transform{Mat4f{1.0}};  // from object space to world space, default is identity
  Transform world_to_obj_ = Transform{Mat4f{1.0}};  // from object space to world space, default is identity
  mutable std::unique_ptr<AABB> bbox_;
};
}