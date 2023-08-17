//
// Created by Bear on 2023/8/2.
//

#include "conductor.h"
#include "fresnel.h"
#include "factory.h"
beart::Spectrum beart::RoughConductor::f(const beart::Vec3f &wo, const beart::Vec3f &wi) const {
  if (!SameHemiSphere(wi, wo)) {
    return 0.;
  }
  if (!PointingExterior(wo)) {  // wo is not pointing to exterior, return
    return 0.f;
  }
  auto L = Spectrum{0., 0., 0.};
  auto h = Normalize(wo + wi);
  float roughness =
      Microfacet::AlphaToRoughness(distribution_type_, alpha_); // most cases alpha_ is identical to roughness
  float D = Microfacet::D(distribution_type_, roughness, h);
  if (LIKELY(D == 0.f)) {
    return L;
  }
  float G = Microfacet::G(distribution_type_, roughness, wo, wi, h);
  auto F = FresnelConductor(Dot(h, wo), eta_, k_);
//  auto F = Spectrum{FresnelConductor(Dot(h, wi), eta_.x(), k_.x()), FresnelConductor(Dot(h, wi), eta_.y(), k_.y()),
//                    FresnelConductor(Dot(h, wi), eta_.z(), k_.z())};
  L = F * G * D * 0.25 / (AbsCosTheta(wo));  // AbsCosTheta(wi) is ignore with render equations
  return L;
}
beart::Spectrum beart::RoughConductor::sample_f(const beart::Vec3f &wo,
                                                beart::Vec3f &wi,
                                                const beart::BsdfSample &bs,
                                                float *pdf) const {

  float roughness =
      Microfacet::AlphaToRoughness(distribution_type_, alpha_); // most cases alpha_ is identical to roughness
  auto m = Microfacet::sample(distribution_type_, roughness, bs);
  wi = Reflect(wo, m);
  if (!PointingExterior(wi) || !SameHemiSphere(wo, wi)) {  // reflection ray is not at the same side of wo
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
  float roughness =
      Microfacet::AlphaToRoughness(distribution_type_, alpha_); // most cases alpha_ is identical to roughness
  auto h = Normalize(wo + wi);
  float D_pdf = Microfacet::pdf(distribution_type_, roughness, h); // p(m) = D(m) * |cos(theta_m)|
  float pdf =
      D_pdf / (4 * AbsDot(wo, h)); // p(wi) = p(m) / (4 * cos(wo, hr))
  return pdf;
}
BEART_REGISTER_CLASS_IN_FACTORY(Bxdf, RoughConductor, "conductor")
