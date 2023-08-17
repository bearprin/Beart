//
// Created by Bear on 2023/07/09.
//

#pragma once

#include "spectrum.h"
#include "samples.h"
#include "json_serializable.h"
namespace beart {
enum BxDFType {
  kBxDF_NONE = 0,
  kGlossyReflection = 1 << 0,
  kGlossyTransmission = 1 << 1,
  kDiffuseReflection = 1 << 2,
  kDiffuseTransmission = 1 << 3,
  kSpecularReflection = 1 << 4,
  kSpecularTransmission = 1 << 5,
  kAnisotropic = 1 << 6,
  kForward = 1 << 7,

  kGlossyLobe = kGlossyReflection | kGlossyTransmission,
  kDiffuseLobe = kDiffuseReflection | kDiffuseTransmission,
  kSpecularLobe = kSpecularReflection | kSpecularTransmission,

  kTransmissiveLobe = kGlossyTransmission | kDiffuseTransmission | kSpecularTransmission,
  kReflectiveLobe = kGlossyReflection | kDiffuseReflection | kSpecularReflection,

  kAllLobes = kTransmissiveLobe | kReflectiveLobe | kAnisotropic,
  kAllButSpecular = ~(kSpecularLobe | kForward),
};
class Bxdf {
 public:
  explicit Bxdf() = default;
  Bxdf(const json &j) : eval_weight_(j.value("eval_weight", 1.0)) {}

  Bxdf(BxDFType type, float eval_weight) : type_(type), eval_weight_(eval_weight) {
  }
  virtual ~Bxdf() = default;
  /// \brief Evaluate the value of Material, just like other render, this function return BxDF * abscos(\theta)
  ///        rather than only BxDF
  /// \param wo
  /// \param wi
  /// \return
  virtual Spectrum F(const Vec3f &wo, const Vec3f &wi) const {
    return f(wo, wi);
  }

  /// \brief Eval, and sampling with pdf
  /// \param wo
  /// \param wi
  /// \param u
  /// \return
  virtual Spectrum SampleF(const Vec3f &wo, Vec3f &wi, const BsdfSample &bs, float *pdf) const {
    return sample_f(wo, wi, bs, pdf);
  }

  /// \brief PDF for important sampling, with cosine importance sampling
  /// \param wo
  /// \param wi
  /// \return
  virtual float Pdf(const Vec3f &wo, const Vec3f &wi) const {
    return pdf(wo, wi);
  }

  float eval_weight() const {
    return eval_weight_;
  }

  void set_geometry_normal(const Vec3f &v) {
    geometry_normal_ = v;
  }

  void set_shading_normal(const Vec3f &v) {
    shading_normal_ = v;
  }

  bool PointingExterior(const Vec3f &wo) const {
    return Dot(wo, geometry_normal_) > 0.f;
  }
  bool SameHemiSphere(const Vec3f &wi, const Vec3f &wo) const {
    return !(PointingExterior(wi) ^ PointingExterior(wo));
  }

 protected:
  virtual Spectrum f(const Vec3f &wo, const Vec3f &wi) const = 0;

  virtual Spectrum sample_f(const Vec3f &wo, Vec3f &wi, const BsdfSample &bs, float *pdf) const = 0;

  virtual float pdf(const Vec3f &wo, const Vec3f &wi) const = 0;

  BxDFType type_ = BxDFType::kBxDF_NONE;
  float eval_weight_ = 1.0f;
  Vec3f geometry_normal_;
  Vec3f shading_normal_;
};
}
