//
// Created by Bear on 2023/8/3.
//

#include "dielectric.h"
#include "fresnel.h"
beart::Spectrum beart::Dielectric::f(const beart::Vec3f &wo, const beart::Vec3f &wi) const {
  float cos_theta_t{};
  // seek wo as the incident ray thus CosTheta(wo) > 0 means ray from outside to inside
  float F = FresnelDielectric(CosTheta(wo), eta_, cos_theta_t);
  if (SameHemiSphere(wi, wo)) {
    return F * AbsCosTheta(wi) * Spectrum(1.f);
  } else if (!SameHemiSphere(wi, wo)) {
    // TODO: LT and BDPT need do not need this
    float factor = CosTheta(wo) > 0 ? eta_ : eta_inv_; // > 0, means outside to inside, use (n2 / n1) ** 2 = eta_ ** 2
    return (1.f - F) * AbsCosTheta(wi) * Spectrum(1.f) * factor * factor;
  }
  return {0.f};
}
beart::Spectrum beart::Dielectric::sample_f(const beart::Vec3f &wo,
                                            beart::Vec3f &wi,
                                            const beart::BsdfSample &bs, float *pdf) const {
  float cos_theta_t{};
  if (float F = FresnelDielectric(CosTheta(wo), eta_, cos_theta_t); bs.u_ < F) {
    wi = Reflect(wo);
    if (pdf) {
      *pdf = F;
    }
  } else {
    wi = Refract(wo, eta_, cos_theta_t);
    if (pdf) {
      *pdf = 1.f - F;
    }
  }
  return f(wo, wi);
}
float beart::Dielectric::pdf(const beart::Vec3f &wo, const beart::Vec3f &wi) const {
  float cos_theta_t{};
  float F = FresnelDielectric(CosTheta(wo), eta_, cos_theta_t);
  if (SameHemiSphere(wi, wo)) {
    return F;
  } else if (!SameHemiSphere(wi, wo)) {
    return (1.f - F);
  }
  return 0.f;

}
