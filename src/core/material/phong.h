//
// Created by Bear on 2022/4/15.
//

#pragma once

#include "bxdf.h"
namespace beart {
///
/// \brief Modified Phong BRDF
/// http://mathinfo.univ-reims.fr/IMG/pdf/Using_the_modified_Phong_reflectance_model_for_Physically_based_rendering_-_Lafortune.pdf
class Phong : public Bxdf {
 public:
  Phong() : Bxdf() {}
 protected:
  Spectrum f(const Vec3f &wo, const Vec3f &wi) const override;
  Spectrum sample_f(const Vec3f &wo, const Vec3f &wi, const BsdfSample &bs, float *pdf) const override;
  float pdf(const Vec3f &wo, const Vec3f &wi) const override;
 private :
  const Spectrum D{1.0f, 0.5f, 0.1f};
  const Spectrum S{0.5f, 0.5f, 0.5f};      /**< Direction-Hemisphere reflectance and transmittance. */
  const float power = 0.6;      /**< Specular power, controlling the specular lobe. */
//  const float diffRatio = ;  /**< Real ratio of diffuse term. */

};
}
