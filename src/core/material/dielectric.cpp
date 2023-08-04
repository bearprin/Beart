//
// Created by Bear on 2023/8/3.
//

#include "dielectric.h"
#include "fresnel.h"
beart::Spectrum beart::Dielectric::f(const beart::Vec3f &wo, const beart::Vec3f &wi) const {
  float eta = CosTheta(wo) < 0 ? eta_ : eta_inv_;   // less than 0 from exr to inter, use in_ior / ext_ior

  float cos_theta_t{};
  float F = FresnelDielectric(AbsCosTheta(wo), eta, cos_theta_t);
  if (SameHemiSphere(wi, wo)) {
    return F * AbsCosTheta(wi) * Spectrum(1.f);
  } else if (!SameHemiSphere(wi, wo)) {
    float factor = eta;
    return (1.f - F) * AbsCosTheta(wi) * Spectrum(1.f) * factor * factor;
  }
  return {0.f};
}
beart::Spectrum beart::Dielectric::sample_f(const beart::Vec3f &wo,
                                            beart::Vec3f &wi,
                                            const beart::BsdfSample &bs, float *pdf) const {
  float eta = CosTheta(wo) < 0 ? eta_ : eta_inv_;
  float cos_theta_t{};
  float F = FresnelDielectric(AbsCosTheta(wo), eta, cos_theta_t);
  if (bs.u_ < F) {
    wi = Reflect(wo);
    if (pdf) {
      *pdf = F;
    }
  } else {
    wi = Refract(wo, eta, cos_theta_t);
    if (pdf) {
      *pdf = 1.f - F;
    }
  }
  return f(wo, wi);
}
float beart::Dielectric::pdf(const beart::Vec3f &wo, const beart::Vec3f &wi) const {
  float eta = CosTheta(wo) < 0 ? eta_ : eta_inv_;   // less than 0 from exr to inter, use in_ior / ext_ior

  float cos_theta_t{};
  float F = FresnelDielectric(AbsCosTheta(wo), eta, cos_theta_t);
  if (SameHemiSphere(wi, wo)) {
    return F;
  } else if (!SameHemiSphere(wi, wo)) {
    return (1.f - F);
  }
  return 0.f;

}
