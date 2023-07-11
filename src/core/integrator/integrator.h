//
// Created by Bear on 2021/12/4.
//

#pragma once

#include "sampler.h"
#include "scene.h"
namespace beart {
class Integrator {
 public:
  virtual ~Integrator() = default;
  virtual Spectrum Li(const Ray &ray, const Scene &scene, const Sampler &sampler) const = 0;
 protected:
  unsigned int max_depth_ = 6;
};
}

