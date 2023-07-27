//
// Created by Bear on 2023/7/27.
//

#pragma once

#include "integrator.h"
namespace beart {
class PathTracing : public Integrator {
 public:
  Spectrum Li(const Ray &ray,
              const Scene &scene,
              const Sampler &sampler,
              Spectrum *normal,
              Spectrum *albedos) const override;

};
}
