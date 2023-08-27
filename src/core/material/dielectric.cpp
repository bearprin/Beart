//
// Created by Bear on 2023/8/8.
//

#include "dielectric.h"
#include "fresnel.h"
#include "factory.h"
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
    float sqrt_dom = Dot(wo, h) + eta * Dot(wi, h);
    float t = eta / sqrt_dom;
    Spectrum value = (1.f - F) * Abs(D * G * AbsCosTheta(wi) * AbsCosTheta(wo) * t * t * Dot(wo, h) * Dot(wi, h)
                                         / (AbsCosTheta(wo)));
    return value;
//    float factor = CosTheta(wo) ? eta : eta_inv_;
//    return Abs(value * factor * factor);
  }
  return {0.f};
}
beart::Spectrum beart::RoughDielectric::sample_f(const beart::Vec3f &wo,
                                                 beart::Vec3f &wi,
                                                 const beart::BsdfSample &bs,
                                                 float *pdf) const {
  float roughness = Microfacet::AlphaToRoughness(distribution_type_, alpha_);
  auto m = Microfacet::sample(distribution_type_, roughness, bs);

  float cos_theta_t{};
  float F = FresnelDielectric(Dot(wo, m), eta_, cos_theta_t);
  if (bs.t_ < F) {
    wi = Reflect(wo, m);
  } else {
    wi = Refract(wo, m, eta_, cos_theta_t);
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
  if (reflect) {
    return D_pdf * F * dwh_dwi;
  } else if (!SameHemiSphere(wi, wo)) {
    return D_pdf * (1.f - F) * dwh_dwi;
  }
  return 0.f;
}
void beart::RoughDielectric::ParseIOR_(const beart::json &j) {
  if (j.contains("in_ior")) {
    if (j["in_ior"].is_string()) {
      in_ior_ = LoadIORData(j["in_ior"].get<std::string>());
    } else {
      in_ior_ = j["in_ior"].get<float>();
    }
  } else {
    in_ior_ = LoadIORData("bk7");
  }
  if (j.contains("ext_ior")) {
    if (j["ext_ior"].is_string()) {
      ext_ior_ = LoadIORData(j["ext_ior"].get<std::string>());
    } else {
      ext_ior_ = j["ext_ior"].get<float>();
    }
  } else {
    ext_ior_ = 1.000277f;
  }
  eta_ = in_ior_ / ext_ior_;
  eta_inv_ = ext_ior_ / in_ior_;
}
beart::Spectrum beart::RoughDielectric::Albedo(const beart::Vec3f &wo, const beart::Vec3f &Ns) {
  float cos_theta_t{};
  return {FresnelDielectric(Dot(wo, Ns), eta_inv_, cos_theta_t)};
}
BEART_REGISTER_CLASS_IN_FACTORY(Bxdf, RoughDielectric, "dielectric")

