//
// Created by Bear on 2023/7/10.
//

#include "direct.h"
#include "interection.h"
#include "integrator_common.h"

beart::Spectrum beart::DirectIntegrator::Li(const beart::Ray &ray,
                                            const beart::Scene &scene,
                                            const beart::Sampler &sampler) const {
  Spectrum L(0.f, 0.f, 0.f);
  if (ray.depth_ > max_depth_) {
    return L;
  }

  // get the intersection between the ray and the scene
  SurfaceInterection info;
  if (!scene.Intersect(ray, &info)) {
    return scene.Le(ray);
  }
  // accumulate the light emitted by intersection itself
  L += info.Le(-ray.dir_);

  Event event(info);
  // evaluate each direct light
  auto lights = scene.lights();
  for (const auto &light : lights) {
    const auto light_sample = LightSample();
    const auto bsdf_sample = BsdfSample();
    L += DirectIllumination(event, ray, scene, *light, light_sample, bsdf_sample);
  }
  return L;
}