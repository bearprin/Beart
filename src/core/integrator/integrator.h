//
// Created by Bear on 2021/12/4.
//

#pragma once

#include "sampler.h"
#include "scene.h"
namespace beart {
class Integrator {
 public:
  Integrator() = default;
  Integrator(const json &j) {
    max_depth_ = j.value("max_depth", 7);
  }
  virtual ~Integrator() = default;
  virtual Spectrum Li(const Ray &ray,
                      const Scene &scene,
                      const Sampler &sampler, Spectrum *normal = nullptr, Spectrum *albedos = nullptr) const = 0;
 protected:
  unsigned int max_depth_ = 7;
};
}

