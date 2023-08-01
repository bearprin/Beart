//
// Created by Bear on 2022/4/15.
//

#include "phong.h"
#include "common.h"
#include "sample_common.h"
#include "transform.h"
#include <numeric>
beart::Spectrum beart::Phong::f(const beart::Vec3f &wo, const beart::Vec3f &wi) const {
  if (!SameHemiSphere(wi, wo)) {
    return 0.;
  }
  if (!PointingExterior(wo)) {  // wo is not pointing to exterior, return
    return 0.f;
  }
  auto L = Spectrum{0.f, 0.f, 0.f};
  L += diffuse_ * kInvPi; // Lambertian diffuse,  f_diffuse( wo , wi ) = D / PI
  // Phong, ( power + 2.0 ) * S * ( ( reflect( wo ) , wi ) ^ power ) / ( 2 * PI )
  if (const auto alpha = SafeDot(wo, Reflect(wi)); alpha > 0.0f) {
    L += specular_ * ((phong_exponent_ + 2) * kInvTwoPi * std::pow(alpha, phong_exponent_));
  }
  return L * AbsCosTheta(wi);
}
beart::Spectrum beart::Phong::sample_f(const beart::Vec3f &wo,
                                       beart::Vec3f &wi,
                                       const beart::BsdfSample &bs,
                                       float *pdf) const {
  auto diff_max_comp = MaxComponent(diffuse_);
  if (bs.u_ < diff_max_comp || diff_max_comp == 1.0f) {
    wi = SampleCosineHemiSphere(bs.u_ / diff_max_comp, bs.v_);
  } else {
    // based on Microfacet models
    const auto cos_theta = std::pow(bs.v_, 1.0f / (phong_exponent_ + 2.0f));
    const auto sin_theta = std::sqrt(std::max(1.0f - cos_theta * cos_theta, 0.f));
    const auto phi = kTwoPi * (bs.u_ - diff_max_comp) / (1.0f - diff_max_comp);
    const auto dir = SphericalVec(sin_theta, cos_theta, phi);
    // naive way
//    const auto alpha = std::acosf(std::pow(bs.v_, 1.0f / (phong_exponent_ + 1.0f)));
//    const auto theta = kTwoPi * bs.u_;
//    const auto dir = SphericalVec(alpha, theta);

    const auto r = Reflect(wo);
    Vec3f t0, t1;
    CoordinateSystem(r, &t0, &t1);
    Transform m{Mat4f{t0.x(), r.x(), t1.x(), 0.0f,
                      t0.y(), r.y(), t1.y(), 0.0f,
                      t0.z(), r.z(), t1.z(), 0.0f,
                      0.0f, 0.0f, 0.0f, 1.0f}};
    wi = m.TransformVector(dir);
  }
  if (pdf) {
    *pdf = this->pdf(wo, wi);
  }
  return f(wo, wi);
}
float beart::Phong::pdf(const beart::Vec3f &wo, const beart::Vec3f &wi) const {
  if (!SameHemiSphere(wi, wo)) {
    return 0.;
  }
  if (!PointingExterior(wo)) {
    return 0.f;
  }
  const auto cos_theta = SafeDot(Reflect(wo), wi);
  const auto pdf_spec = pow(cos_theta, phong_exponent_ + 1.0f) * (phong_exponent_ + 2.0f) * kInvTwoPi;
  const auto pdf_diff = SampleCosineHemiSpherePdf(wi);
  return Lerp(pdf_spec, pdf_diff, MaxComponent(diffuse_));  // interpolate diffuse and specular
}
