//
// Created by Bear on 2022/4/15.
//

#pragma once

#include "bxdf.h"
#include "interection.h"
namespace beart {
class Event {
 public:
  Event(const SurfaceInterection &info);
  /// \brief Evaluate each bxdf considering its evaluate_weight
  /// \param wo
  /// \param wi
  /// \param
  Spectrum EvaluateBxDF(const Vec3f &wo, const Vec3f &wi) const;
  /// \brief Transform v to shading coordinate, we use shade normal as axis y
  /// \param v
  /// \return
  Vec3f WorldToLocal(const Vec3f &v) const;
  /// \brief Transform v from shading coordinate to world coordinate
  /// \param v
  /// \return
  Vec3f Local2World(const Vec3f &v) const;

  const SurfaceInterection &info() const {
    return info_;
  }
  float Pdf(const Vec3f &wo, const Vec3f &wi) const;
  Spectrum SampleF(const Vec3f &wo, Vec3f &wi, const BsdfSample &bs, float *pdf) const;
 private:
  Vec3f n_;
  Vec3f b_;
  Vec3f t_;
  Vec3f local_ng_;
  Vec3f local_ns_ = Vec3f{0, 0, 1};
  const SurfaceInterection &info_;
  const std::vector<std::shared_ptr<Bxdf>> *bxdfs_;
};
}
