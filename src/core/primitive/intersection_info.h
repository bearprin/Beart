//
// Created by Bear on 2021/12/3.
//

#pragma once

#include "common.h"
#include "spectrum.h"
namespace beart {
class IntersectionInfo {
 public:
  bool query_shadow = false;
  bool happened = false;
  float t_curr = kMaxFloat;
  Vec3f view; // view direction
  Vec3f corrds; // intersection point
  Vec3f Ng;  // Face Normal
  Vec3f Ns;  // Shading Normal
  Vec3f tangent; // tangent direction
  Vec2f uv; // texture coordinate

  const Primitive *primitive_ = nullptr;
  /// Le for ray directly intersect with the light
  /// \param wo
  /// \return
  Spectrum Le(const Vec3f &wo) const;
};
}