//
// Created by Bear on 2021/12/4.
//

#ifndef BEART_SRC_CORE_MATH_QUADRATIC_H_
#define BEART_SRC_CORE_MATH_QUADRATIC_H_

#include "common.h"

namespace beart {
inline std::tuple<bool, float, float> SolveQuadratic(const float &a, const float &b, const float &c) {
  float discr = b * b - 4.f * a * c;
  if (discr < 0) {
    return {false, std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity()};
  } else if (discr == 0) {
    return {true, -0.5 * b / a, -0.5 * b / a};
  } else {
    float q = (b > 0) ? -0.5f * (b + std::sqrtf(discr)) : -0.5f * (b - std::sqrtf(discr));
    float x_0 = q / a;
    float x_1 = c / q;
    if (x_0 > x_1) {
      std::swap(x_0, x_1);
    }
    return {true, x_0, x_1};
  }
};
}

#endif //BEART_SRC_CORE_MATH_QUADRATIC_H_
