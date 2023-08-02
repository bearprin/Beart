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
                          DistributionType type = DistributionType::GGX,
                          float eval_weight = 1.0)
      : name_(name), alpha_(alpha), type_(type),
        Bxdf(static_cast<BxDFType>(BxDFType::kBxDF_GLOSSY),
             eval_weight) {
    auto res = LoadCondutorData(name_);
    eta_ = std::get<0>(res);
    k_ = std::get<1>(res);
  }
 protected:
  Spectrum f(const Vec3f &wo, const Vec3f &wi) const override;
  Spectrum sample_f(const Vec3f &wo, Vec3f &wi, const BsdfSample &bs, float *pdf) const override;
  float pdf(const Vec3f &wo, const Vec3f &wi) const override;
 private:
  std::string name_;
  float alpha_ = 0.1; // roughness
  DistributionType type_ = DistributionType::GGX;
  Spectrum eta_; // index of refraction
  Spectrum k_;  // extinction coefficient
};
}
