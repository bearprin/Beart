//
// Created by Bear on 2023/7/12.
//

#include "diffuse.h"
#include "common.h"
#include "sample_common.h"
#include "factory.h"
beart::Spectrum beart::Diffuse::f(const beart::Vec3f &wo, const beart::Vec3f &wi) const {
  if (!SameHemiSphere(wo, wi)) {
    return {0.0f, 0.f, 0.f};
  }
  if (!PointingExterior(wo)) {  // wo is not pointing to exterior, return
    return 0.f;
  }
  return diffuse_ * kInvPi * AbsCosTheta(wi);
}
beart::Spectrum beart::Diffuse::sample_f(const beart::Vec3f &wo,
                                         beart::Vec3f &wi,
                                         const beart::BsdfSample &bs,
                                         float *pdf) const {
  // sample diffuse hemisphere
  wi = SampleCosineHemiSphere(bs.u_ / MaxComponent(diffuse_), bs.v_);
  // record pdf
  if (pdf) {
    *pdf = this->pdf(wo, wi);
  }
  // return f
  return f(wo, wi);
}
float beart::Diffuse::pdf(const beart::Vec3f &wo, const beart::Vec3f &wi) const {
  if (!SameHemiSphere(wo, wi)) {
    return 0.f;
  }
  if (!PointingExterior(wo)) {
    return 0.f;
  }
  const auto pdf_diff = SampleCosineHemiSpherePdf(wi);
  return pdf_diff;
}
BEART_REGISTER_CLASS_IN_FACTORY(Bxdf, Diffuse, "lambertian")


