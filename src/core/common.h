//
// Created by Bear on 2021/12/3.
//

#pragma once

#include <vector>

#include <memory>
#include <utility>

#include <cmath>

namespace beart {
static constexpr float kMinFloat = std::numeric_limits<float>::min();
static constexpr float kMaxFloat = std::numeric_limits<float>::max();
static constexpr float kPi = 3.1415926535897932384626433832795028;

static std::tuple<bool, float, float> SolveQuadratic(const float &a, const float &b, const float &c) {
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
}
}