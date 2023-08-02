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
}
