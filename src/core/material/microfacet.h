//
// Created by Bear on 2023/7/31.
//

#pragma once

#include "vec.h"
#include "samples.h"
namespace beart {
enum class DistributionType {
  Beckmann,
  Phong,
  GGX,
  // TODO: add more complex distributions
};
class Microfacet {
  // Microfacet Models for Refraction through Rough Surfaces
 public:
  static float D(DistributionType distribution_type, const float alpha, const Vec3f &m) {
    // n is the normal of the microsurface
    if (CosTheta(m) <= 0.0f) {
      return 0.0f;
    }
    switch (distribution_type) {
      case DistributionType::Beckmann:return D_Beckmann(alpha, m);
      case DistributionType::Phong:return D_Phong(alpha, m);
      case DistributionType::GGX:return D_GGX(alpha, m);
    }
    return 0.0f;
  }
  static float G(DistributionType distribution_type,
                 const float alpha,
                 const Vec3f &wi,
                 const Vec3f &wo,
                 const Vec3f &m) {
    return G1(distribution_type, alpha, wi, m) * G1(distribution_type, alpha, wo, m);
  }
  static float G1(DistributionType distribution_type, const float alpha, const Vec3f &v, const Vec3f &m) {
    if (Dot(v, m) / CosTheta(v) <= 0.0f) {
      return 0.0f;
    }
    switch (distribution_type) {
      case DistributionType::Beckmann:return G1_Beckmann(alpha, v);
      case DistributionType::Phong:return G1_Phong(alpha, v);
      case DistributionType::GGX:return G1_GGX(alpha, v);
    }
    return 0.0f;
  }
  static float pdf(DistributionType distribution_type, const float alpha, const Vec3f &m) {
    return D(distribution_type, alpha, m) * AbsCosTheta(m); // pdf(m) = D(m) * |m * n|
  }
  static Vec3f sample(DistributionType distribution_type,
                      const float alpha,
                      const BsdfSample &bs) {
    float phi = kTwoPi * bs.v_;
    float cos_theta{};
    switch (distribution_type) {
      case DistributionType::Beckmann: {
        float tan_theta_2 = -alpha * alpha * std::log(1 - bs.u_);
        cos_theta = 1.f / std::sqrt(1 + tan_theta_2); // 1 + tan_theta_2 = sec2theta
        break;
      }
      case DistributionType::Phong: {
        cos_theta = std::pow(bs.u_, 1 / (alpha + 2));
        break;
      }
      case DistributionType::GGX: {
        float tan_theta_2 = alpha * alpha * bs.u_ / (1 - bs.u_);
        cos_theta = 1.f / std::sqrt(1 + tan_theta_2); // 1 + tan_theta_2 = sec2theta
        break;
      }
    }
    float sin_theta = std::sqrt(std::max(1 - cos_theta * cos_theta, 0.f));
    return SphericalVec(sin_theta, cos_theta, phi);
  }
  static float AlphaToRoughness(DistributionType distribution_type, const float alpha) {
    if (distribution_type == DistributionType::Phong) {
      return std::sqrt(2.0f / (alpha + 1.0f));
    }
    return alpha;
  }

 private:
  static float D_Beckmann(const float alpha, const Vec3f &m) {
    float cos_theta = CosTheta(m);
    float cos_theta_2 = cos_theta * cos_theta;
    float cos_theta_4 = cos_theta_2 * cos_theta_2;
    float tan_theta_2 = std::max(1.0f - cos_theta_2, 0.f) / cos_theta_2;
    float alpha_2 = alpha * alpha;
    return std::exp(-tan_theta_2 / alpha_2) / (alpha_2 * cos_theta_4) * kInvPi;
  }
  static float D_Phong(const float alpha, const Vec3f &m) {
    float cos_theta = CosTheta(m);
    return (alpha + 2.0f) * kInvTwoPi * std::pow(cos_theta, alpha);
  }
  static float D_GGX(const float alpha, const Vec3f &m) {
    float cos_theta = CosTheta(m);
    float cos_theta_2 = cos_theta * cos_theta;
    float cos_theta_4 = cos_theta_2 * cos_theta_2;
    float tan_theta_2 = std::max(1.0f - cos_theta_2, 0.f) / cos_theta_2;
    float alpha_2 = alpha * alpha;
    return alpha_2 * kInvPi / (cos_theta_4 * (alpha_2 + tan_theta_2) * (alpha_2 + tan_theta_2));
  }
  static float G1_Beckmann(const float alpha, const Vec3f &v) {
    // approximation of the shadowing-masking function
    float cos_theta = CosTheta(v);
    float tan_theta = std::sqrtf(std::max(1.0f - cos_theta * cos_theta, 0.f)) / cos_theta;
    float a = 1.0f / (alpha * tan_theta);
    if (a >= 1.6f) {
      return 1.0f;
    }
    return (3.535f * a + 2.181f * a * a) / (1.0f + 2.276f * a + 2.577f * a * a);
  }
  static float G1_Phong(const float alpha, const Vec3f &v) {
    // approximation of the shadowing-masking function
    float cos_theta = CosTheta(v);
    float tan_theta = std::sqrtf(std::max(1.0f - cos_theta * cos_theta, 0.f)) / cos_theta;
    float a = std::sqrtf(0.5f * alpha + 1.0f) * tan_theta;
    if (a >= 1.6f) {
      return 1.0f;
    }
    return (3.535f * a + 2.181f * a * a) / (1.0f + 2.276f * a + 2.577f * a * a);
  }
  static float G1_GGX(const float alpha, const Vec3f &v) {
    float cos_theta_2 = CosTheta(v) * CosTheta(v);
    float tan_theta_2 = std::max(1.0f - cos_theta_2, 0.0f) / cos_theta_2;
    return 2.f / (1.f + std::sqrtf(1.f + alpha * alpha * tan_theta_2));
  }

};

}