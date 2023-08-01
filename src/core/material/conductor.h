//
// Created by Bear on 2023/8/1.
//

#pragma once

#include "bxdf.h"

#include "conductor_data.h"
namespace beart {
class Conductor : public Bxdf {
 public:
  explicit Conductor(std::string_view name, float eval_weight = 1.0) : name_(name),
                                                                       Bxdf(static_cast<BxDFType>(BxDFType::kBXDF_REFLECTION),
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
  std::string name_ = "Cu";
  Spectrum eta_; // index of refraction
  Spectrum k_;  // extinction coefficient

};
}
