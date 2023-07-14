//
// Created by Bear on 2023/7/10.
//

#pragma once

#include "integrator.h"
namespace beart {
class NormalIntegrator : public Integrator {
  public:
    Spectrum Li(const Ray &ray, const Scene &scene, const Sampler &sampler) const override;

};
}
