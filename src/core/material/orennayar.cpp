//
// Created by Bear on 2023/8/27.
//

#include "orennayar.h"
#include "sample_common.h"
#include "factory.h"
beart::Spectrum beart::Orennayar::f(const beart::Vec3f &wo, const beart::Vec3f &wi) const {
  if (!SameHemiSphere(wi, wo)) {
    return {0.0f, 0.f, 0.f};
  }
  if (!PointingExterior(wo)) {  // wo is not pointing to exterior, return
    return 0.f;
  }
  float roughness_to_sigma = 1.f / std::sqrt(2.f);
  float sigma = alpha_ * roughness_to_sigma;
  float sigma_2 = sigma * sigma;

  float theta_i = SafeAcos(wi);
  float theta_o = SafeAcos(wo);
  float alpha = std::max(theta_i, theta_o);
  float beta = std::min(theta_i, theta_o);
  float sin_alpha = std::sin(alpha);

  float denom = (wi.x() * wi.x() + wi.y() * wi.y()) * (wo.x() * wo.x() + wo.y() * wo.y());
  float cos_diff{};
  if (denom == 0.0f) {
    cos_diff = 1.0f;
  } else {
    cos_diff = (wi.x() * wo.x() + wi.y() * wo.y()) / std::sqrt(denom);
  }

  float C1 = 1 - 0.5f * sigma_2 / (sigma_2 + 0.33f);
  float C2 = 0.45f * sigma_2 / (sigma_2 + 0.09f);
  if (cos_diff >= 0.f) {
    C2 *= sin_alpha;
  } else {
    C2 = C2 * (sin_alpha - std::powf(2.f * beta * kInvPi, 3.f));
  }
  float C3 =
      0.125f * sigma_2 / (sigma_2 + 0.09f) * (4.f * alpha * beta * kInvPi * kInvPi)
          * (4.f * alpha * beta * kInvPi * kInvPi);
  float C4 = 0.17f * sigma_2 / (sigma_2 + 0.13f);

  auto f1 = (C1 + cos_diff * C2 * std::tan(beta) + (1.f - std::abs(cos_diff)) * C3 * std::tan(0.5f * (alpha + beta)))
      * diffuse_;
  auto f2 = C4 * (1.f - cos_diff * (2 * beta * kInvPi) * (2 * beta * kInvPi)) * diffuse_ * diffuse_;
  return (f1 + f2) * AbsCosTheta(wi) * kInvPi;
}
beart::Spectrum beart::Orennayar::sample_f(const beart::Vec3f &wo,
                                           beart::Vec3f &wi,
                                           const beart::BsdfSample &bs,
                                           float *pdf) const {
  if (bs.t_ < alpha_) {
    wi = SampleUniformHemiSphere(bs.u_ / MaxComponent(diffuse_), bs.v_);
  } else {
    wi = SampleCosineHemiSphere(bs.u_ / MaxComponent(diffuse_), bs.v_);
  }
  // record pdf
  if (pdf) {
    *pdf = this->pdf(wo, wi);
  }
  // return f
  return f(wo, wi);
}
float beart::Orennayar::pdf(const beart::Vec3f &wo, const beart::Vec3f &wi) const {
  if (!SameHemiSphere(wi, wo)) {
    return 0.f;
  }
  if (!PointingExterior(wo)) {
    return 0.f;
  }
  const auto pdf_uniform = SampleUniformHemiSpherePdf();
  const auto pdf_cosine = SampleCosineHemiSpherePdf(wi);
  return pdf_uniform * alpha_ + pdf_cosine * (1.0f - alpha_);
}
beart::Spectrum beart::Orennayar::Albedo(const beart::Vec3f &wo, const beart::Vec3f &Ns) {
  return diffuse_;
}
BEART_REGISTER_CLASS_IN_FACTORY(Bxdf, Orennayar, "oren")
