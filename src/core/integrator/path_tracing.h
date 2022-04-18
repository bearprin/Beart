//
// Created by Bear on 2022/4/12.
//

#pragma once

#include "integrator.h"
namespace beart {
class PathTracing : public Integrator {
 public:
  Spectrum Li(const Ray &ray, const Scene &scene, const PixelSample &sampler) const override;
 private:
  Spectrum li(const Ray &ray, const Scene &scene, const PixelSample &sampler, int bounces) const;
};
}

