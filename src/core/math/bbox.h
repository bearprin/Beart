//
// Created by Bear on 2023/7/6.
//

#pragma once

#include "vec.h"
#include "ray.h"
namespace beart {
enum class BBoxType {
  kAABB = 1
};
class BBox {
 public:
  virtual float HalfSurfaceArea() const = 0;
  virtual float SurfaceArea() const = 0;
  virtual float Volume() const = 0;
  virtual Point3f Centroid() const = 0;
  virtual unsigned int MaxAxisExtents() const = 0;
  virtual bool Intersect(const Ray &ray) const = 0;
  virtual bool IsInBox(const Point3f &p) const = 0;

  virtual BBoxType GetBBoxType() const = 0;

};
} // namespace beart
