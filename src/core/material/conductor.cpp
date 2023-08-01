//
// Created by Bear on 2023/8/1.
//

#include "conductor.h"
#include "fresnel.h"
beart::Spectrum beart::Conductor::f(const beart::Vec3f &wo, const beart::Vec3f &wi) const {
  if (!SameHemiSphere(wi, wo)) {
    return 0.;
  }
  if (!PointingExterior(wo)) {  // wo is not pointing to exterior, return
    return 0.f;
  }
  auto L = FresnelConductor(CosTheta(wi), eta_, k_);
  return L * AbsCosTheta(wi);
}
beart::Spectrum beart::Conductor::sample_f(const beart::Vec3f &wo,
                                           beart::Vec3f &wi,
                                           const beart::BsdfSample &bs,
                                           float *pdf) const {
  wi = Reflect(wo);  // perfect reflection
  if (pdf) {
    *pdf = this->pdf(wo, wi);
  }
  return f(wo, wi);
}
float beart::Conductor::pdf(const beart::Vec3f &wo, const beart::Vec3f &wi) const {
  if (!SameHemiSphere(wi, wo)) {
    return 0.;
  }
  if (!PointingExterior(wo)) {
    return 0.f;
  }
  return 1.f;  // perfect reflection
}
