//
// Created by Bear on 2022/4/12.
//

#include "path_tracing.h"
#include "integrator_common.h"

beart::Spectrum beart::PathTracing::Li(const beart::Ray &ray,
                                       const beart::Scene &scene,
                                       const beart::PixelSample &sampler) const {
  return li(ray, scene, sampler, 1);
}
beart::Spectrum beart::PathTracing::li(const beart::Ray &ray,
                                       const beart::Scene &scene,
                                       const beart::PixelSample &sampler,
                                       int bounces) const {
  Spectrum L{0., 0., 0.};
//  int local_bounces = 0;
//  while (true) {
//    if (bounces > max_depth_) {
//      return L;
//    }
//    IntersectionInfo info;
//    if (!scene.IntersectInfo(ray, &info)) {
//      return L;
//    }
//    // accumulate the light emitted by intersection itself
//    if (local_bounces == 0) {
//      L += info.Le(-ray.dir());
//    }
//    // direct illumination
//    auto light_pdf = 0.0f;
//    const auto light_sample = LightSample();
//    const auto bsdf_sample = BsdfSample();
//    const auto light =
//        scene.SampleLight(light_sample.t_, &light_pdf);  // sample one light from multi-lights for direct illumination
//    if (light_pdf > 0.0f) {
//      L += DirectIllumination(ray, info, scene, light, light_sample, bsdf_sample) / light_pdf;
//    }
//
//    bounces += 1;
//    local_bounces += 1;
//    // Russian roulette
//  }
  return L;
}
