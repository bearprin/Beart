//
// Created by Bear on 2021/12/3.
//

#pragma once

#include "ray.h"

#include <Eigen/Eigen>
#include <iostream>

namespace beart {
using Mat4f = Eigen::Matrix4f;

using Mat3f = Eigen::Matrix3f;

static inline Mat4f Inverse(const Mat4f &m) {
  auto d = m.determinant();
  return m.inverse();
//  d = 1.f / d;
//  float res[16];
//  res[0] = m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] +
//           m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
}
}