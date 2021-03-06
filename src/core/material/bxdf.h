//
// Created by Bear on 2022/4/15.
//

#pragma once

#include "spectrum.h"
#include "samples.h"
namespace beart {
enum BxDFType {
  kBxDF_NONE = 0,
  kBxDF_DIFFUSE = 1,
  kBxDF_GLOSSY = 2,
  kBXDF_REFLECTION = 8,
  kBxDF_TRANSMISSION = 16,
  kBXDF_ALL_TYPES = kBxDF_DIFFUSE | kBxDF_GLOSSY,
  kBXDF_ALL_REFLECTION = kBXDF_ALL_TYPES | kBXDF_REFLECTION,
};
class Bxdf {
 public:
  explicit Bxdf() = default;
  Bxdf(BxDFType type, float eval_weight) : type_(type), eval_weight_(eval_weight) {
  }
  virtual ~Bxdf() = default;
  /// \brief Evaluate the value of Material, just like other render, this function return BXDF * abscos(\theta)
  ///        rather than only BXDF
  /// \param wo
  /// \param wi
  /// \return
  virtual Spectrum F(const Vec3f &wo, const Vec3f &wi) const {
    return f(wo, wi);
  }

  /// \brief BxDF Sampling
  /// \param wo
  /// \param wi
  /// \param u
  /// \return
  virtual Spectrum SampleF(const Vec3f &wo, const Vec3f &wi, const BsdfSample &bs, float *pdf) const {
    return sample_f(wo, wi, bs, pdf);
  }

  /// \brief PDF for important sampling
  /// \param wo
  /// \param wi
  /// \return
  virtual float Pdf(const Vec3f &wo, const Vec3f &wi) const {
    return pdf(wo, wi);
  }

  [[nodiscard]] float eval_weight() const {
    return eval_weight_;
  }

  void set_geometry_normal(const Vec3f &v) {
    geometry_normal_ = v;
  }

  void set_shading_normal(const Vec3f &v) {
    shading_normal_ = v;
  }

 protected:
  virtual Spectrum f(const Vec3f &wo, const Vec3f &wi) const = 0;

  virtual Spectrum sample_f(const Vec3f &wo, const Vec3f &wi, const BsdfSample &bs, float *pdf) const = 0;

  virtual float pdf(const Vec3f &wo, const Vec3f &wi) const = 0;

  BxDFType type_ = BxDFType::kBxDF_NONE;
  float eval_weight_ = 1.0f;
  Vec3f geometry_normal_;
  Vec3f shading_normal_;
};
}
