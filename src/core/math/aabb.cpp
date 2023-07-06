//
// Created by Bear on 2023/7/6.
//

#include "aabb.h"
float beart::AABB::SurfaceArea() const {
  return 0;
}
float beart::AABB::Volume() const {
  return 0;
}
bool beart::AABB::Intersect(const beart::Ray &ray) const {
  return false;
}
bool beart::AABB::IsInBox(const beart::Point3f &p) const {
  return false;
}
