//
// Created by Bear on 2023/8/2.
//

#pragma once

#include "bxdf.h"
#include "microfacet.h"
#include "conductor_data.h"
namespace beart {
class RoughConductor : public Bxdf {
 public:
  explicit RoughConductor(std::string_view name,
                          float alpha = 0.1,
                          DistributionType type = DistributionType::kGGX,
                          float eval_weight = 1.0)
      : Bxdf(BxDFType::kGlossyReflection,
             eval_weight), name_(name), alpha_(alpha),
        distribution_type_(type) {
    auto [eta, k] = LoadCondutorData(name_);
    eta_ = eta;
    k_ = k;
  }
  RoughConductor(const json &j) : RoughConductor(j.value("material_name", "Cu"),
                                                 j.value("alpha", 0.1),
                                                 kDistributionTypeMap[j.value("distribution_type", "ggx")],
                                                 j.value("eval_weight", 1.0)) {

  }
 protected:
  Spectrum f(const Vec3f &wo, const Vec3f &wi) const override;
  Spectrum sample_f(const Vec3f &wo, Vec3f &wi, const BsdfSample &bs, float *pdf) const override;
  float pdf(const Vec3f &wo, const Vec3f &wi) const override;
 private:
  std::string name_;
  float alpha_ = 0.1; // roughness
  DistributionType distribution_type_ = DistributionType::kGGX;
  Spectrum eta_; // index of refraction
  Spectrum k_;  // extinction coefficient
};
}
