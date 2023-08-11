//
// Created by Bear on 2023/8/8.
//

#pragma once

#include "bxdf.h"
#include "ior_data.h"
#include "microfacet.h"
namespace beart {
class RoughDielectric : public Bxdf {
 public:
  RoughDielectric(const float in_ior,
                  const float ext_ior = 1.000277f,
                  const float alpha = 0.1,
                  DistributionType type = DistributionType::kGGX,
                  const float eval_weight = 1.0f)
      : Bxdf(static_cast<BxDFType>(BxDFType::kGlossyReflection | BxDFType::kGlossyTransmission), eval_weight),
        distribution_type_(type),
        alpha_(alpha),
        in_ior_(in_ior),
        ext_ior_(ext_ior) {
    eta_ = in_ior_ / ext_ior_; // eta = in_ior / ext_ior
    eta_inv_ = ext_ior_ / in_ior_;
  }
  RoughDielectric(std::string_view in_ior,
                  std::string_view ext_ior = "air",
                  const float alpha = 0.1,
                  DistributionType type = DistributionType::kGGX,
                  const float eval_weight = 1.0f) : Bxdf(
      static_cast<BxDFType>(BxDFType::kGlossyReflection | BxDFType::kGlossyTransmission),
      eval_weight), distribution_type_(type), alpha_(alpha) {
    in_ior_ = LoadIORData(in_ior);
    ext_ior_ = LoadIORData(ext_ior);
    eta_ = in_ior_ / ext_ior_;
    eta_inv_ = ext_ior_ / in_ior_;
  }
 protected:
  Spectrum f(const Vec3f &wo, const Vec3f &wi) const override;
  Spectrum sample_f(const Vec3f &wo, Vec3f &wi, const BsdfSample &bs, float *pdf) const override;
  float pdf(const Vec3f &wo, const Vec3f &wi) const override;
 private:
  DistributionType distribution_type_ = DistributionType::kGGX;
  float alpha_ = 0.1f;
  float in_ior_;
  float ext_ior_ = 1.000277f;
  float eta_ = 1.f; // eta = in_ior / ext_ior
  float eta_inv_ = 0.f; // eta_inv = ext_ior / in_ior

};
}
