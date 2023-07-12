//
// Created by Bear on 2022/4/15.
//

#include "phong.h"
#include "bxdf_common.h"
#include "common.h"
beart::Spectrum beart::Phong::f(const beart::Vec3f &wo, const beart::Vec3f &wi) const {
  if (CosTheta(wo) <= 0.0f || CosTheta(wi) <= 0.0f) { // wo and wi are on different side of surface, return
    return {0.0f, 0.f, 0.f};
  }
  auto L = Spectrum{0.f, 0.f, 0.f};
  L += diffuse_ * kInvPi; // Lambertian diffuse,  f_diffuse( wo , wi ) = D / PI
  // Phong, ( power + 2.0 ) * S * ( ( reflect( wo ) , wi ) ^ power ) / ( 2 * PI )
  if (const auto alpha = SafeDot(wo, Reflect(wi)); alpha > 0.0f) {
    L += specular_ * ((phong_exponent_ + 2) * kInvTwoPi * std::pow(alpha, phong_exponent_));
  }
  return L * AbsCosTheta(wi);
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
