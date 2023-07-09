//
// Created by Bear on 2023/7/6.
//

#pragma once

#include <enoki/array.h>

namespace beart {
using Vec4f = enoki::Array<float, 4>;

using Vec3f = enoki::Array<float, 3>;
using Vec3i = enoki::Array<int, 3>;
using Vec3u = enoki::Array<uint32_t, 3>;
using Vec3b = enoki::Array<uint8_t, 3>;

using Vec2f = enoki::Array<float, 2>;

using Point3f = enoki::Array<float, 3>;
using Point3i = enoki::Array<int, 3>;
using Point3u = enoki::Array<uint32_t, 3>;
using Point3b = enoki::Array<uint8_t, 3>;

using Point4f = enoki::Array<float, 4>;

inline static
float Dot(const Vec3f &lhs, const Vec3f &rhs) {
  return enoki::dot(lhs, rhs);
}
inline static
Vec3f Cross(const Vec3f &lhs, const Vec3f &rhs) {
  return enoki::cross(lhs, rhs);
}
inline static
Vec3f Normalize(const Vec3f &v) {
  return enoki::normalize(v);
}
inline static
Vec3f Clamp(const Vec3f &v, float min, float max) {
  return enoki::clamp(v, min, max);
}
inline static
float Norm(const Vec3f &v) {
  return enoki::norm(v);
}
inline static
Point3f Min(const Point3f &lhs, const Point3f &rhs) {
  return enoki::min(lhs, rhs);
}
inline static
Point3f Max(const Point3f &lhs, const Point3f &rhs) {
  return enoki::max(lhs, rhs);
}
inline static
void CoordinateSystem(const Vec3f &v1, Vec3f *v2, Vec3f *v3) {
  // TODO: leverage <<Building an Orthonormal Basis, Revisited>>
  auto v = Normalize(v1);
  // Construct linear independent vector v2
  if (std::abs(v.x()) > std::abs(v.y())) {
    *v3 = Vec3f(v.z(), 0, -v.x()) / std::sqrt(v.x() * v.x() + v.z() * v.z());
  } else {
    *v3 = Vec3f(0, v.z(), -v.y()) / std::sqrt(v.y() * v.y() + v.z() * v.z());
  }
  *v2 = Normalize(Cross(*v3, v));
}
}

