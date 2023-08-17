//
// Created by Bear on 2023/07/10.
//

#pragma once

#include "integrator.h"
#include "sampler.h"
namespace beart {

class AOIntegrator : public Integrator {
 public:
  AOIntegrator() = default;
  AOIntegrator(const json &j) : Integrator(j), max_distance(j.value("max_distance", 10.0f)) {

  }
  Spectrum Li(const Ray &ray,
              const Scene &scene,
              const Sampler &sampler,
              Spectrum *normal,
              Spectrum *albedos) const override;
 private:
  float max_distance = 10.0f;

};
}
