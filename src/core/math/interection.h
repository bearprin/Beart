//
// Created by Bear on 2023/7/6.
//

#pragma once

#include "common.h"
#include "vec.h"
namespace beart {
struct SurfaceInterection {
  float t_curr = kMaxFloat;

  Vec3f Ng;  // geometric normal
  Vec3f Ns;  // shading normal
  Vec3f tangent;
  Vec3f view;
  Point3f intersect_pos;
};
}