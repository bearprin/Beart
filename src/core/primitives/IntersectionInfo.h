//
// Created by Bear on 2021/12/3.
//

#ifndef BEART_SRC_CORE_PRIMITIVES_INTERSECTIONINFO_H_
#define BEART_SRC_CORE_PRIMITIVES_INTERSECTIONINFO_H_

#include "Vec.h"

namespace beart {
class Primitive;
class IntersectionInfo {
 public:
  bool happened = false;
  float t_curr = 0.0;
  Vec3f corrds;
  Vec3f Ng;
  Vec3f Ns;

  const Primitive *obj = nullptr;

};
}

#endif //BEART_SRC_CORE_PRIMITIVES_INTERSECTIONINFO_H_
