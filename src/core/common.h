//
// Created by Bear on 2023/7/6.
//

#pragma once
#include <limits>
#include <tuple>
#include <filesystem>
#include <fstream>
#include <numeric>

#include <cmath>
namespace beart {
namespace fs = std::filesystem;
// forward declare
class Visibility;
class Primitive;
class Light;
class Scene;
class Camera;
class Integrator;
class Sampler;
class Accelerator;
class Shape;
// used for gcc and clang
// if-else predication
#define UNLIKELY(EXP)       __builtin_expect((EXP),0)
#define LIKELY(EXP)         __builtin_expect((EXP),1)

#if defined(__GNUC__) || defined(__GNUG__) || defined(__clang__)
#define BERT_FORCEINLINE  __attribute__((always_inline))
#elif defined(_MSC_VER)
#define BERT_FORCEINLINE __forceinline
#endif

static constexpr float kMinFloat = std::numeric_limits<float>::lowest();
static constexpr float kMaxFloat = std::numeric_limits<float>::max();
static constexpr float kInfinity = std::numeric_limits<float>::infinity();
static constexpr float kPi = 3.1415926535897932384626433832795028f;
static constexpr float kInvPi = 0.3183098861837906715377675267450287f;
static constexpr float kTwoPi = 6.28318530717948769f;
static constexpr float kInvTwoPi = 0.159154943091895335768883763372514f;
static constexpr float kFourPi = 12.566370614359172f;
static constexpr float kInvFourPi = 0.079577471545947667884441881686257f;

static constexpr float kEpsilon = 1e-4;

template<typename IN_T, typename OUT_T>
static
constexpr OUT_T reinterpret_type(const IN_T in) {
// Good compiler should optimize memcpy away.
  OUT_T out;
  memcpy(&out, &in, sizeof(out));
  return out;
}
static
constexpr float AddUlpMagnitude(float f, int ulps) {
  if (!std::isfinite(f)) return f;
  const unsigned bits = reinterpret_type<float, unsigned>(f);
  return reinterpret_type<unsigned, float>(bits + ulps);
}
static
constexpr float DegToRad(float deg) { return deg * kPi / 180.f; }

static
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
static
constexpr float Lerp(const float a, const float b, const float t) {
  return (1 - t) * a + t * b;
}
static
constexpr float SafeSqrt(float x) {
  return std::sqrt(std::max(0.0f, x));
}
}