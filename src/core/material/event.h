//
// Created by Bear on 2022/4/15.
//

#pragma once

#include "vec.h"
#include "bxdf.h"
#include "intersection_info.h"
namespace beart {
class Event {
 public:
  Event(const IntersectionInfo &info);
  /// \brief Evaluate each bxdf considering its evaluate_weight
  /// \param wo
  /// \param wi
  /// \param
  [[nodiscard]] Spectrum EvaluateBxDF(const Vec3f &wo, const Vec3f &wi) const;
  /// \brief Transform v to shading coordinate, we use shade normal as axis y
  /// \param v
  /// \return
  [[nodiscard]] Vec3f World2Local(const Vec3f &v) const;
  /// \brief Transform v from shading coordinate to world coordinate
  /// \param v
  /// \return
  [[nodiscard]] Vec3f Local2World(const Vec3f &v) const;

  [[nodiscard]] const IntersectionInfo &info() const {
    return info_;
  }
 private:
//  std::array<const Bxdf *, kMaxBxdf> bxdfs_;
  unsigned int bxdfs_count_ = 0;
  Vec3f n_;
  Vec3f b_;
  Vec3f t_;
  const IntersectionInfo &info_;
  Bxdf *bxdf_;
};
}
