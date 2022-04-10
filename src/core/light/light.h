//
// Created by Bear on 2021/12/3.
//

#pragma once

#include "spectrum.h"
namespace beart {
class Light {
 public:
  ~Light() = default;
 protected:
  Spectrum intensity_;

};
}
