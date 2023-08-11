//
// Created by Bear on 2023/8/8.
//

#include "rough_dielectric.h"
#include "fresnel.h"
beart::Spectrum beart::RoughDielectric::f(const beart::Vec3f &wo, const beart::Vec3f &wi) const {
  bool reflect = SameHemiSphere(wi, wo);
  auto h = Vec3f{};
  float eta = CosTheta(wo) > 0 ? eta_ : eta_inv_;  // wo > 0, means outside to inside, use (n2 / n1)
  if (reflect) {
    h = Normalize(wo + wi);
  } else {
    h = Normalize(wo + eta * wi);
  }
  if (h.z() < 0.) { // if h is not in the same hemisphere with Ng, so flip it
    h = -h;
  }
  float roughness = Microfacet::AlphaToRoughness(distribution_type_, alpha_);
  auto D = Microfacet::D(distribution_type_, roughness, h);
  auto G = Microfacet::G(distribution_type_, roughness, wi, wo, h);
  float cos_theta_t{};
  float F = FresnelDielectric(Dot(wo, h), eta_, cos_theta_t);
  if (reflect) {
    return F * D * G / (4 * AbsCosTheta(wo));
  } else if (!SameHemiSphere(wi, wo)) {
    float factor = eta; // > 0, means outside to inside, use (n2 / n1) ** 2 = eta_ ** 2
    float sqrt_dom = Dot(wo, h) + eta * Dot(wi, h);
    float t = factor / sqrt_dom;
    return (1.f - F) * D * G * AbsCosTheta(wi) * AbsCosTheta(wo) * t * t * Dot(wo, h) * Dot(wi, h)
        / (AbsCosTheta(wo));
  }
  return {0.f};
}
beart::Spectrum beart::RoughDielectric::sample_f(const beart::Vec3f &wo,
                                                 beart::Vec3f &wi,
                                                 const beart::BsdfSample &bs,
                                                 float *pdf) const {
  bool reflect = SameHemiSphere(wi, wo);
  auto h = Vec3f{};
  float eta = CosTheta(wo) > 0 ? eta_ : eta_inv_;  // wo > 0, means outside to inside, use (n2 / n1)
  float dwh_dwi{};
  if (reflect) {
    h = Normalize(wo + wi);
    dwh_dwi = 1.0 / (4 * AbsDot(wo, h));
  } else {
    h = Normalize(wo + eta * wi);
    float sqrt_dom = Dot(wo, h) + eta * Dot(wi, h);
    dwh_dwi = eta * eta * AbsDot(wi, h) / (sqrt_dom * sqrt_dom);
  }
  if (h.z() < 0.) { // if h is not in the same hemisphere with Ng, so flip it
    h = -h;
  }
  float roughness = Microfacet::AlphaToRoughness(distribution_type_, alpha_);
  auto m = Microfacet::sample(distribution_type_, roughness, bs);

  float cos_theta_t{};
  float F = FresnelDielectric(Dot(wo, h), eta_, cos_theta_t);
  if (bs.t_ < F) {
    wi = Reflect(wo, m);
  }
  else {
    wi = Refract(wo, m, eta, cos_theta_t);
  }
  if (pdf) {
    *pdf = this->pdf(wo, wi);
  }
  return f(wo, wi);
}
float beart::RoughDielectric::pdf(const beart::Vec3f &wo, const beart::Vec3f &wi) const {
  bool reflect = SameHemiSphere(wi, wo);
  auto h = Vec3f{};
  float eta = CosTheta(wo) > 0 ? eta_ : eta_inv_;  // wo > 0, means outside to inside, use (n2 / n1)
  float dwh_dwi{};
  if (reflect) {
    h = Normalize(wo + wi);
    dwh_dwi = 1.0 / (4 * AbsDot(wo, h));
  } else {
    h = Normalize(wo + eta * wi);
    float sqrt_dom = Dot(wo, h) + eta * Dot(wi, h);
    dwh_dwi = eta * eta * AbsDot(wi, h) / (sqrt_dom * sqrt_dom);
  }
  if (h.z() < 0.) { // if h is not in the same hemisphere with Ng, so flip it
    h = -h;
  }
  float roughness = Microfacet::AlphaToRoughness(distribution_type_, alpha_);
  auto D_pdf = Microfacet::pdf(distribution_type_, roughness, h);
  float cos_theta_t{};
  float F = FresnelDielectric(Dot(wo, h), eta_, cos_theta_t);
  float dwi_dwh = 1.f / (4 * Dot(wo, h));
  if (reflect) {
    return D_pdf * F * dwi_dwh;
  } else if (!SameHemiSphere(wi, wo)) {
    return D_pdf * (1.f - F) * dwh_dwi;
  }
  return 0.f;
}
