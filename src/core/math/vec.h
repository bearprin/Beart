//
// Created by Bear on 2021/12/3.
//

#pragma once

#include <Eigen/Eigen>

namespace beart {
//template<typename T, int size>
//class Vec {
//  [[nodiscard]] inline T &x() const {
//    return impl_.x();
//  }
//  [[nodiscard]] inline T &y() const {
//    return impl_.y();
//  }
//  [[nodiscard]] inline T &z() const {
//    return impl_.z();
//  }
//  [[nodiscard]] inline const T *data() const {
//    return impl_.data();
//  }
//  inline float Norm() const {
//    return impl_.norm();
//  }
//  inline float SquareNorm() const {
//    return impl_.squaredNorm();
//  }
//  inline float Dot(const Vec<T, size> &rhs) {
//    return impl_.dot(rhs.impl_);
//  }
//  inline Vec<T, size> Cross(const Vec<T, size> &rhs) {
//    Vec<T, size> res = rhs.Cross(rhs);
//    return res;
//  }
// private:
//  Eigen::Vector<T, size> impl_;
//};

using Vec4f = Eigen::Vector4f;

using Vec3f = Eigen::Vector3f;
using Vec3d = Eigen::Vector3d;
using Vec3i = Eigen::Vector3i;
using Vec3u = Eigen::Vector<unsigned int, 3>;

using Vec2f = Eigen::Vector2f;
using Vec2d = Eigen::Vector2d;
using Vec2i = Eigen::Vector2i;
using Vec2u = Eigen::Vector<unsigned int, 2>;

static inline float Dot(const Vec3f &lhs, const Vec3f &rhs) {
  float res = lhs.dot(rhs);
  return res;
}
static inline Vec3f Cross(const Vec3f &lhs, const Vec3f &rhs) {
  Vec3f res = lhs.cross(rhs);
  return res;
}
static inline Vec3f Normalize(const Vec3f &v) {
  Vec3f res = v.normalized();
  return res;
}
inline void CoordinateSystem(const Vec3f &v1, Vec3f *v2, Vec3f *v3) {
  auto v = v1.normalized();
  // Construct linear independent vector v2
  if (std::abs(v.x()) > std::abs(v.y())) {
    *v3 = Vec3f(v.z(), 0, -v.x()) / std::sqrt(v.x() * v.x() + v.z() * v.z());
  } else {
    *v3 = Vec3f(0, v.z(), -v.y()) / std::sqrt(v.y() * v.y() + v.z() * v.z());
  }
  *v2 = Normalize(Cross(*v3, v));
}
}