//
// Created by Bear on 2023/07/10.
//

#pragma once

#include <utility>

#include "bxdf.h"
namespace beart {
/// \brief Modified Phong BRDF
/// <<Using_the_modified_Phong_reflectance_model_for_Physically_based_rendering>>
class Phong : public Bxdf {
 public:
  explicit Phong(Spectrum diffuse = {0.5},
                 Spectrum specular = {0.2f},
                 const float phong_exponent = 30.,
                 const float eval_weight = 1.0)
      : Bxdf(static_cast<BxDFType>(BxDFType::kDiffuseReflection | BxDFType::kGlossyReflection), eval_weight),
        diffuse_(std::move(diffuse)),
        specular_(std::move(specular)),
        phong_exponent_(phong_exponent) {
    assert(diffuse_.x() + specular_.x() <= 1.0f);
    assert(diffuse_.y() + specular_.y() <= 1.0f);
    assert(diffuse_.z() + specular_.z() <= 1.0f);
  }
 protected:
  [[nodiscard]] Spectrum f(const Vec3f &wo, const Vec3f &wi) const override;
  Spectrum sample_f(const Vec3f &wo, Vec3f &wi, const BsdfSample &bs, float *pdf) const override;
  [[nodiscard]] float pdf(const Vec3f &wo, const Vec3f &wi) const override;
 private :
  const Spectrum diffuse_;
  const Spectrum specular_;
  const float phong_exponent_; // phong exponent, higher value give more mirror-like specular reflection
};
}
