//
// Created by Bear on 2023/8/1.
//

#pragma once

#include "common.h"
#include "spectrum.h"
namespace beart {
// Note that conductor and dielectric are same equation for fresnel in theory
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
static BERT_FORCEINLINE float FresnelDielectric(float cos_theta_i, float eta, float &cos_theta_t) {
  if (cos_theta_i < 0) { // ray from inside to outside
    eta = 1.0f / eta;
    cos_theta_i = -cos_theta_i; // cos_theta_i is always positive
  }
  float sin_theta_i = std::sqrt(std::max(0.0f, 1.0f - cos_theta_i * cos_theta_i));
  float sin_theta_t = eta * sin_theta_i;
  if (sin_theta_t >= 1) {
    cos_theta_t = 0.f;
    return 1;
  }
  cos_theta_t = std::sqrt(std::max(0.0f, 1.0f - sin_theta_t * sin_theta_t));
  float r_parl = (eta * cos_theta_i - cos_theta_t) / (eta * cos_theta_i + cos_theta_t);
  float r_perp = (cos_theta_i - eta * cos_theta_t) / (cos_theta_i + eta * cos_theta_t);
  return (r_parl * r_parl + r_perp * r_perp) / 2;
}
}
