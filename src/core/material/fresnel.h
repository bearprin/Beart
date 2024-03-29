//
// Created by Bear on 2023/8/1.
//

#pragma once

#include "common.h"
#include "spectrum.h"
namespace beart {
// Note that conductor and dielectric are same equation for fresnel in theory
static BERT_FORCEINLINE float FresnelConductor(float cos_theta,
                                               const float &eta,
                                               const float &k) {
  float cosThetaI2 = cos_theta * cos_theta;
  float sinThetaI2 = 1 - cosThetaI2;
  float sinThetaI4 = sinThetaI2 * sinThetaI2;

  float temp1 = eta * eta - k * k - sinThetaI2;
  float a2pb2 = SafeSqrt(temp1 * temp1 + 4 * k * k * eta * eta);
  float a = SafeSqrt(0.5f * (a2pb2 + temp1));

  float term1 = a2pb2 + cosThetaI2;
  float term2 = 2 * a * cos_theta;

  float Rs2 = (term1 - term2) / (term1 + term2);

  float term3 = a2pb2 * cosThetaI2 + sinThetaI4;
  float term4 = term2 * sinThetaI2;

  float Rp2 = Rs2 * (term3 - term4) / (term3 + term4);

  return 0.5f * (Rp2 + Rs2);
}
static BERT_FORCEINLINE Spectrum FresnelConductor(float cos_theta,
                                                  const Spectrum &eta,
                                                  const Spectrum &k) {
  // snell 's law to get sin_theta_t and replace all cos to sin
  // compute rp^2, rs^2 directly
  float cos_theta_2 = cos_theta * cos_theta;
  Spectrum t = 2 * eta * cos_theta;
  Spectrum tmp_f = eta * eta + k * k;
  Spectrum tmp = tmp_f * cos_theta_2;
  Spectrum Rparl2 = (tmp - t + 1) / (tmp + t + 1);
  Spectrum Rperp2 = (tmp_f - t + cos_theta_2) / (tmp_f + t + cos_theta_2);
  return (Rparl2 + Rperp2) * 0.5f;
}
///
/// \param cos_theta_i_
/// \param eta in_ior / ext_ior
/// \param cos_theta_t_
/// \return
static BERT_FORCEINLINE float FresnelDielectric(float cos_theta_i_, float eta, float &cos_theta_t_) {
  // larger than 0, means outside to inside, switch in_ior/ext_ior to ext_ior/in_ior
  float scale = (cos_theta_i_ > 0) ? 1 / eta : eta;
  float cos_theta_t_2 = 1 - (1 - cos_theta_i_ * cos_theta_i_) * (scale * scale);

  if (cos_theta_t_2 <= 0.0f) {
    cos_theta_t_ = 0.0f;
    return 1.0f;
  }
  // cos_theta_i always to be positive
  float cos_theta_i = std::abs(cos_theta_i_);
  float cos_theta_t = std::sqrt(cos_theta_t_2);

  float Rs = (cos_theta_i - eta * cos_theta_t)
      / (cos_theta_i + eta * cos_theta_t);
  float Rp = (eta * cos_theta_i - cos_theta_t)
      / (eta * cos_theta_i + cos_theta_t);

  // refraction will change the different sign compared to cos_theta_i
  cos_theta_t_ = (cos_theta_i_ > 0) ? -cos_theta_t : cos_theta_t;

  return 0.5f * (Rs * Rs + Rp * Rp);
}
}
