//
// Created by Bear on 2023/8/2.
//

#include "rough_conductor.h"
#include "fresnel.h"
beart::Spectrum beart::RoughConductor::f(const beart::Vec3f &wo, const beart::Vec3f &wi) const {
  if (!SameHemiSphere(wi, wo)) {
    return 0.;
  }
  if (!PointingExterior(wo)) {  // wo is not pointing to exterior, return
    return 0.f;
  }
  auto L = Spectrum{0., 0., 0.};
  auto h = Normalize(wo + wi);
  float roughness = Microfacet::AlphaToRoughness(type_, alpha_); // most cases alpha_ is identical to roughness
  float D = Microfacet::D(type_, roughness, h);
  if (LIKELY(D == 0.f)) {
    return L;
  }
  float G = Microfacet::G(type_, roughness, wo, wi, h);
  auto F = FresnelConductor(Dot(h, wi), eta_, k_);
  L = F * G * D * 0.25 / (AbsCosTheta(wo));  // AbsCosTheta(wi) is ignore with render equations
  return L;
}
beart::Spectrum beart::RoughConductor::sample_f(const beart::Vec3f &wo,
                                                beart::Vec3f &wi,
                                                const beart::BsdfSample &bs,
                                                float *pdf) const {

  float roughness = Microfacet::AlphaToRoughness(type_, alpha_); // most cases alpha_ is identical to roughness
  auto m = Microfacet::sample(type_, roughness, bs);
  wi = Reflect(wo, m);
  if (!PointingExterior(wi)) {  // reflection ray is not at the same side of wo
    if (pdf) {
      *pdf = 0;
    }
    return {0.f};
  }
  if (pdf) {
    *pdf = this->pdf(wo, wi);
  }
  return f(wo, wi);
}
float beart::RoughConductor::pdf(const beart::Vec3f &wo, const beart::Vec3f &wi) const {
  if (!SameHemiSphere(wi, wo)) {
    return 0.;
  }
  if (!PointingExterior(wo)) {
    return 0.f;
  }
  float roughness = Microfacet::AlphaToRoughness(type_, alpha_); // most cases alpha_ is identical to roughness
  auto h = Normalize(wo + wi);
  float D_pdf = Microfacet::pdf(type_, roughness, h); // p(m) = D(m) * |cos(theta_m)|
  float pdf =
      D_pdf / (4 * SafeDot(wi, h)); // p(wi) = p(m) / (4 * cos(wi, hr))
  return pdf;
}
