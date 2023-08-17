//
// Created by Bear on 2023/7/12.
//

#pragma once
#include "bxdf.h"
#include "json_serializable.h"
namespace beart {
class Diffuse : public Bxdf {
 public:
  Diffuse() = default;
  Diffuse(Spectrum reflectance, const float eval_weight = 1.0)
      : Bxdf(static_cast<BxDFType>(BxDFType::kDiffuseReflection), eval_weight),
        diffuse_(std::move(reflectance)) {
  }
  Diffuse(const json &j) : Diffuse(j.value("reflectance", Spectrum{0.5f}), j.value("eval_weight", 1.0)) {
  }
 protected:
  Spectrum f(const Vec3f &wo, const Vec3f &wi) const override;
  Spectrum sample_f(const Vec3f &wo, Vec3f &wi, const BsdfSample &bs, float *pdf) const override;
  float pdf(const Vec3f &wo, const Vec3f &wi) const override;
 private :
  Spectrum diffuse_ = Spectrum{0.5f};

};
}
