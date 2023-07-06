//
// Created by Bear on 2023/7/6.
//

#pragma once

#include <enoki/matrix.h>
namespace beart {
using Mat3f = enoki::Matrix<float, 3>;
using Mat4f = enoki::Matrix<float, 4>;

inline static
Mat3f Mat4ftoMat3f(const Mat3f &matrix) {
  return Mat3f{
      matrix(0, 0), matrix(0, 1), matrix(0, 2),
      matrix(1, 0), matrix(1, 1), matrix(1, 2),
      matrix(2, 0), matrix(2, 1), matrix(2, 2),
  };
}
inline static
Mat4f Inverse(const Mat4f &m) {
  return enoki::inverse(m);
}
inline static
Mat4f InverseTranspose(const Mat4f &m) {
  return enoki::inverse_transpose(m);
}
}
