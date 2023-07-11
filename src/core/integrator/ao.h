//
// Created by Bear on 2023/07/10.
//

#pragma once

#include "integrator.h"
#include "sampler.h"
namespace beart {

class AOIntegrator : public Integrator {
 public:
  Spectrum Li(const Ray &ray, const Scene &scene, const Sampler &sampler) const override;
 private:
  float max_distance = 10.0f;

};
}
