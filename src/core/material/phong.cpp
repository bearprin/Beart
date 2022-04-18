//
// Created by Bear on 2022/4/15.
//

#include "phong.h"
#include "bxdf_common.h"
#include "common.h"
beart::Spectrum beart::Phong::f(const beart::Vec3f &wo, const beart::Vec3f &wi) const {
//  if (!IsSameSide(wo, geometry_normal_)) {
//    return {0.0f, 0.f, 0.f};
//  }
//  if (!IsSameHemisphere(wo, wi, geometry_normal_)) {
//    return {0.f, 0.f, 0.f};
//  }
  auto L = Spectrum{0.f, 0.f, 0.f};
  // Diffuse  : f_diffuse( wo , wi ) = D / PI
  L += D * kInvPi;
  // Specular : f_specular( wo , wi ) = ( power + 2.0 ) * S * ( ( reflect( wo ) , wi ) ^ power ) / ( 2 * PI )
  if (const auto alpha = SafeDot(wi, Reflect(wo)); alpha > 0.0f) {
    L += S * (power + 2) * std::pow(alpha, power) * kInvTwoPi;
  }
  return L;
}
beart::Spectrum beart::Phong::sample_f(const beart::Vec3f &wo,
                                       const beart::Vec3f &wi,
                                       const beart::BsdfSample &bs,
                                       float *pdf) const {
  return beart::Spectrum();
}
float beart::Phong::pdf(const beart::Vec3f &wo, const beart::Vec3f &wi) const {
  return 0;
}
