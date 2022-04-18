//
// Created by Bear on 2022/4/14.
//

#pragma once

#include "integrator.h"
namespace beart {
class Direct : public Integrator {
 public:
  virtual Spectrum Li(const Ray &ray, const Scene &scene, const PixelSample &sampler) const override;

};
}

