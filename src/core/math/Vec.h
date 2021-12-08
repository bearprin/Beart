//
// Created by Bear on 2021/12/3.
//

#ifndef BEART_SRC_CORE_MATH_VEC_H_
#define BEART_SRC_CORE_MATH_VEC_H_

#include <Eigen/Eigen>

namespace beart {
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
  return std::move(res);
}
}

#endif //BEART_SRC_CORE_MATH_VEC_H_
