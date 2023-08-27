//
// Created by Bear on 2023/8/27.
//

#pragma once

#include "bxdf.h"
namespace beart {
class Orennayar : public Bxdf {
 public:
  Orennayar(Spectrum reflectance, float alpha, const float eval_weight = 1.0)
      : Bxdf(static_cast<BxDFType>(BxDFType::kDiffuseReflection), eval_weight),
        diffuse_(std::move(reflectance)),
        alpha_(std::move(alpha)) {

  }
  Orennayar(const json &j) : Orennayar(j.value("reflectance", Spectrum{0.5f}),
                                       j.value("alpha", 0.2f),
                                       j.value("eval_weight", 1.0)) {
  }
  Spectrum Albedo(const Vec3f &wo, const Vec3f &Ns) override;
 protected:
  Spectrum f(const Vec3f &wo, const Vec3f &wi) const override;
  Spectrum sample_f(const Vec3f &wo, Vec3f &wi, const BsdfSample &bs, float *pdf) const override;
  float pdf(const Vec3f &wo, const Vec3f &wi) const override;
 private:
  Spectrum diffuse_ = Spectrum{0.5f};
  float alpha_ = 0.2f;

};
}
