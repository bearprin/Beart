//
// Created by Bear on 2023/7/10.
//

#pragma once

#include "integrator.h"
namespace beart {
class DirectIntegrator : public Integrator {
 public:
  DirectIntegrator() = default;
  DirectIntegrator(const json &j) : Integrator(j) {}
  virtual Spectrum Li(const Ray &ray,
                      const Scene &scene,
                      const Sampler &sampler,
                      Spectrum *normal = nullptr,
                      Spectrum *albedos = nullptr) const override;

};
}

