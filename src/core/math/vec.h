//
// Created by Bear on 2023/7/6.
//

#pragma once

#include "common.h"
#include <enoki/array.h>

namespace beart {
template<class T, unsigned long N>
using Vec = enoki::Array<T, N>;

using Vec4f = enoki::Array<float, 4>;

using Vec3f = enoki::Array<float, 3>;
using Vec3i = enoki::Array<int, 3>;
using Vec3u = enoki::Array<uint32_t, 3>;
using Vec3b = enoki::Array<uint8_t, 3>;

using Vec2f = enoki::Array<float, 2>;
using Vec2i = enoki::Array<int, 2>;

using Point3f = enoki::Array<float, 3>;
using Point3i = enoki::Array<int, 3>;
using Point3u = enoki::Array<uint32_t, 3>;
using Point3b = enoki::Array<uint8_t, 3>;

using Point4f = enoki::Array<float, 4>;

using Point2f = enoki::Array<float, 2>;

BERT_FORCEINLINE static
float Dot(const Vec3f &lhs, const Vec3f &rhs) {
  return enoki::dot(lhs, rhs);
}
BERT_FORCEINLINE static
float AbsDot(const Vec3f &lhs, const Vec3f &rhs) {
  return std::abs(enoki::dot(lhs, rhs));
}
BERT_FORCEINLINE static
Vec3f Cross(const Vec3f &lhs, const Vec3f &rhs) {
  return enoki::cross(lhs, rhs);
}
BERT_FORCEINLINE static
Vec3f Normalize(const Vec3f &v) {
  return enoki::normalize(v);
}
BERT_FORCEINLINE static
Vec3f Clamp(const Vec3f &v, float min, float max) {
  return enoki::clamp(v, min, max);
}
BERT_FORCEINLINE static
float Norm(const Vec3f &v) {
  return enoki::norm(v);
}
BERT_FORCEINLINE static
float SquareNorm(const Vec3f &v) {
  return enoki::squared_norm(v);
}
BERT_FORCEINLINE static
Point3f Min(const Point3f &lhs, const Point3f &rhs) {
  return enoki::min(lhs, rhs);
}
BERT_FORCEINLINE static
Point3f Max(const Point3f &lhs, const Point3f &rhs) {
  return enoki::max(lhs, rhs);
}
BERT_FORCEINLINE static
void CoordinateSystem(const Vec3f &v1, Vec3f *v2, Vec3f *v3) {
  // <<Building an Orthonormal Basis, Revisited>>
  auto v = Normalize(v1);
  float sign = std::copysignf(1.0f, v.z());
  float a = -1.0f / (sign + v.z());
  float b = v.x() * v.y() * a;
  *v2 = Vec3f(1.0f + sign * v.x() * v.x() * a, sign * b, -sign * v.x());
  *v3 = Vec3f(b, sign + v.y() * v.y() * a, -v.y());

//  auto v = Normalize(v1);
//  // Construct linear independent vector v2
//  if (std::abs(v.x()) > std::abs(v.y())) {
//    *v3 = Vec3f(v.z(), 0, -v.x()) / std::sqrt(v.x() * v.x() + v.z() * v.z());
//  } else {
//    *v3 = Vec3f(0, v.z(), -v.y()) / std::sqrt(v.y() * v.y() + v.z() * v.z());
//  }
//  *v2 = Normalize(Cross(*v3, v));
}

//BERT_FORCEINLINE static
//bool IsSameSide(const Vec3f &v1, const Vec3f &ng) {
//   return CosTheta(v1) > 0.f;
//}
//BERT_FORCEINLINE static
//bool IsSameHemisphere(const Vec3f &v1, const Vec3f &v2, const Vec3f &ng) {
//  return IsSameSide(v1, ng) * IsSameSide(v2, ng) > 0.f;
//}
static BERT_FORCEINLINE float SafeDot(const Vec3f &v1, const Vec3f &v2) {
  return std::clamp(Dot(v1, v2), 0.0f, 1.0f);
}
/// @brief Compute the cosine of the angle between two vectors.
/// shading coordinate optimization (ns as axis z)
/// \param v
/// \param ns
/// \return
static BERT_FORCEINLINE float AbsCosTheta(const Vec3f &v) {
  return fabsf(v.z());
}
static BERT_FORCEINLINE float CosTheta(const Vec3f &v) {
  return v.z();
}
static BERT_FORCEINLINE float SafeAcos(const Vec3f &v) {
  return std::acos(std::clamp(v.z(), -1.0f, 1.0f));
}
static BERT_FORCEINLINE float SafeAcos(const float &v) {
  return std::acos(std::clamp(v, -1.0f, 1.0f));
}
static BERT_FORCEINLINE float SinTheta(const Vec3f &v) {
  return std::sqrt(std::max(0.f, 1.f - v.z() * v.z()));
}
/// shading coordinate optimization (Ns as axis z)
/// \param v
/// \return
static BERT_FORCEINLINE beart::Vec3f Reflect(const Vec3f &v) {
  return {-v.x(), -v.y(), v.z()};
}
/// shading coordinate optimization (Ns as axis z)
static BERT_FORCEINLINE beart::Vec3f Refract(const Vec3f &v, float eta, float cos_theta_t) {
  // eta = n2 / n1
  float scale = -(cos_theta_t < 0 ? 1. / eta : eta);  // cos_theta_t < 0 means from outside to inside
  return {v.x() * scale, v.y() * scale, cos_theta_t};
}
// General coordinate system implementation
static BERT_FORCEINLINE beart::Vec3f Reflect(const Vec3f &v, const Vec3f &n) {
  return 2. * Dot(v, n) * n - v;
}
static BERT_FORCEINLINE beart::Vec3f Refract(const Vec3f &v, const Vec3f &n, float eta, float cos_theta_t) {
  // eta = n2 / n1
  if (cos_theta_t < 0) {
    eta = 1 / eta;
  }
  return n * (Dot(v, n) * eta + cos_theta_t) - v * eta;
}
static BERT_FORCEINLINE beart::Vec3f Lerp(const Vec3f &v1, const Vec3f &v2, float t) {
  return v1 * (1 - t) + v2 * t;
}
static BERT_FORCEINLINE beart::Vec3f Abs(const Vec3f &v) {
  return {std::abs(v.x()), std::abs(v.y()), std::abs(v.z())};
}
static BERT_FORCEINLINE unsigned int MaxDim(const Vec3f &v) {
  return v.x() > v.y() ? (v.x() > v.z() ? 0 : 2) : (v.y() > v.z() ? 1 : 2);
}
static BERT_FORCEINLINE float MaxComponent(const Vec3f &v) {
  return std::max({v.x(), v.y(), v.z()});
}
static BERT_FORCEINLINE float MinComponent(const Vec3f &v) {
  return std::min({v.x(), v.y(), v.z()});
}
static BERT_FORCEINLINE beart::Point3f Permute(const beart::Point3f &p, int x, int y, int z) {
  return {p[x], p[y], p[z]};
}
static BERT_FORCEINLINE beart::Vec3f FaceForward(const beart::Vec3f &n, const beart::Vec3f &v) {
  return Dot(n, v) < 0.f ? -n : n;
}
static BERT_FORCEINLINE beart::Vec3f SphericalVec(float sintheta, float costheta, float phi) {
  float x = sintheta * cos(phi);
  float y = sintheta * sin(phi);
  float z = costheta;
  return beart::Vec3f{x, y, z};
}
static BERT_FORCEINLINE beart::Vec3f SphericalVec(float theta, float phi) {
  float x = sinf(theta) * cosf(phi);
  float y = sinf(theta) * sinf(phi);
  float z = cosf(theta);
  return beart::Vec3f{x, y, z};
}
}
