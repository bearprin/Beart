//
// Created by Bear on 2023/7/6.
//

#pragma once
#include <limits>
#include <tuple>

#include <cmath>
namespace beart {
// used for gcc and clang
// if-else predication
#define UNLIKELY(EXP)       __builtin_expect((EXP),0)
#define LIKELY(EXP)         __builtin_expect((EXP),1)

static constexpr float kMinFloat = std::numeric_limits<float>::lowest();
static constexpr float kMaxFloat = std::numeric_limits<float>::max();
static constexpr float kInfinity = std::numeric_limits<float>::infinity();
static constexpr float kPi = 3.1415926535897932384626433832795028f;
static constexpr float kInvPi = 0.3183098861837906715377675267450287f;
static constexpr float kTwoPi = 6.28318530717948769f;
static constexpr float kInvTwoPi = 0.159154943091895335768883763372514f;

static constexpr float kEpsilon = 1e-6f;
static constexpr unsigned int kMaxBxdf = 8;

inline static
constexpr std::tuple<bool, float, float> SolveQuadratic(const float &a, const float &b, const float &c) {
  float discr = b * b - 4.f * a * c;
  if (discr < 0) {
    return {false, kInfinity, kInfinity};
  } else if (discr == 0) {
    return {true, -0.5 * b / a, -0.5 * b / a};
  } else {
    float q = (b > 0) ? -0.5f * (b + std::sqrt(discr)) : -0.5f * (b - std::sqrt(discr));
    float x_0 = q / a;
    float x_1 = c / q;
    if (x_0 > x_1) {
      std::swap(x_0, x_1);
    }
    return {true, x_0, x_1};
  }
};
}