//
// Created by Bear on 2021/12/3.
//

#pragma once

#include "vec.h"

namespace beart {
class Primitive;
class intersectionInfo {
 public:
  bool happened = false;
  float t_curr = 0.0;
  Vec3f corrds;
  Vec3f Ng;
  Vec3f Ns;

  const Primitive *obj = nullptr;

};
}