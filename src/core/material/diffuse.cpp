//
// Created by Bear on 2023/7/12.
//

#include "diffuse.h"
#include "common.h"
#include "sample_common.h"
beart::Spectrum beart::Diffuse::f(const beart::Vec3f &wo, const beart::Vec3f &wi) const {
  if (CosTheta(wo) <= 0.0f || CosTheta(wi) <= 0.0f) { // wo and wi are on different side of surface, return
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
  wi = SampleCosineHemiSphere(bs.u_ / diffuse_.x(), bs.v_);
  // record pdf
  if (pdf) {
    *pdf = this->pdf(wo, wi);
  }
  // return f
  return f(wo, wi);
}
float beart::Diffuse::pdf(const beart::Vec3f &wo, const beart::Vec3f &wi) const {
  if (CosTheta(wo) <= 0.0f || CosTheta(wi) <= 0.0f) { // wo and wi are on different side of surface, return
    return 0.f;
  }
  if (!PointingExterior(wo)) {
    return 0.f;
  }
  const auto pdf_diff = SampleCosineHemiSpherePdf(wi);
  return pdf_diff;
}


