//
// Created by Bear on 2023/7/10.
//

#pragma once

#include "vec.h"
#include "common.h"

#include <utility>
#include <cmath>
namespace beart {
BERT_FORCEINLINE static
std::pair<float, float> SampleUniformDisk(const float u, const float v) {
  float r = std::sqrtf(u);
  float theta = 2 * kPi * v;
  return {r * std::cosf(theta), r * std::sinf(theta)};
}
BERT_FORCEINLINE static
Vec3f SampleUniformSphere(const float u, const float v) {
  float z = 1 - 2 * u;
  float r = std::sqrtf(std::fmax(0.f, 1.f - z * z));  // safe sqrt
  float phi = 2 * kPi * v;
  return {r * std::cosf(phi), r * std::sinf(phi), z};
}
BERT_FORCEINLINE static
Vec3f SampleUniformHemiSphere(const float u, const float v) {
  float z = u;
  float r = std::sqrtf(std::fmax(0.f, 1.f - z * z));  // safe sqrt
  float phi = 2 * kPi * v;
  return {r * std::cosf(phi), r * std::sinf(phi), z};
}
BERT_FORCEINLINE static
Vec3f SampleCosineHemiSphere(const float u, const float v) {
  // Malley’s method: sampling cosine-weighted hemisphere by projection
  auto [x, y] = SampleUniformDisk(u, v);
  float z = std::sqrtf(std::fmax(0.f, 1.f - x * x - y * y));  // Ns as z axis
  return {x, y, z};
}
BERT_FORCEINLINE static
Vec3f SampelCosineSphere(const float u, const float v) {
  float z = 1 - 2 * u;
  float r = std::sqrtf(std::fmax(0.f, 1.f - z * z));  // safe sqrt
  float phi = 2 * kPi * v;
  return {r * std::cosf(phi), r * std::sinf(phi), z};
}
BERT_FORCEINLINE static
Vec3f SampleUniformCone(const float u, const float v, const float costhetamax) {
  float costheta = (1.f - u) + u * costhetamax;
  float sintheta = std::sqrtf(1.f - costheta * costheta);
  float phi = v * 2.f * kPi;
  return {std::cosf(phi) * sintheta, std::sinf(phi) * sintheta, costheta};
}
BERT_FORCEINLINE static
float SampleUniformDiskPdf() {
  return kInvPi;
}
BERT_FORCEINLINE static
float SampleUniformSpherePdf() {
  return kInvFourPi;
}
BERT_FORCEINLINE static
float SampleUniformHemiSpherePdf() {
  return kInvTwoPi;
}
BERT_FORCEINLINE static
float SampleCosineSpherePdf(Vec3f v) {
  return v.z() * kInvPi;
}
BERT_FORCEINLINE static
float SampleCosineHemiSpherePdf(Vec3f v) {
  return AbsCosTheta(v) * kInvPi;
}
BERT_FORCEINLINE static
float SampleUniformConePdf(const float costhetamax) {
  return 1.f / (2.f * kPi * (1.f - costhetamax));
}
}
