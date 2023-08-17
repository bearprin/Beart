//
// Created by Bear on 2023/8/11.
//

#pragma once

#include "integrator.h"
#include "bidpath/path_vertex.h"
namespace beart {
class BidPathTracing : public Integrator {
 public:
  BidPathTracing() = default;
  BidPathTracing(const json &j) : Integrator(j) {
    light_tracing_only_ = j.value("light_tracing_only", false);
    sample_per_pixel_ = j.value("sample_per_pixel", 1);
  }
  BidPathTracing(bool light_tracing_only, unsigned int sample_per_pixel = 1)
      : light_tracing_only_(light_tracing_only), sample_per_pixel_(sample_per_pixel) {}
  Spectrum Li(const Ray &ray,
              const Scene &scene,
              const Sampler &sampler,
              Spectrum *normal,
              Spectrum *albedos) const override;
 private:
  void ConnectCamera(const beart::PathVertex &light_vertex,
                     const beart::Light &light,
                     const beart::Scene &scene,
                     const beart::Sampler &sampler) const;
  Spectrum ConnectLight(const beart::PathVertex &camera_vertex,
                        const beart::Light &light,
                        const beart::Scene &scene,
                        const beart::Sampler &sampler) const;
  Spectrum ConnectVertices(const beart::PathVertex &light_vertex,
                           const beart::PathVertex &camera_vertex,
                           const beart::Light &light,
                           const beart::Scene &scene,
                           const beart::Sampler &sampler) const;
 protected:
  bool light_tracing_only_ = false;  // only do light tracing
  unsigned int sample_per_pixel_ = 1; // light sample per pixel

};
}
