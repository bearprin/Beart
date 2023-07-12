//
// Created by Bear on 2023/7/10.
//

#pragma once

#include "integrator.h"
namespace beart {
class DirectIntegrator : public Integrator {
 public:
  virtual Spectrum Li(const Ray &ray, const Scene &scene, const Sampler &sampler) const override;

};
}

