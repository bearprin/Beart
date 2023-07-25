//
// Created by Bear on 2023/7/10.
//

#include "direct.h"
#include "interection.h"
#include "integrator_common.h"

beart::Spectrum beart::DirectIntegrator::Li(const beart::Ray &ray,
                                            const beart::Scene &scene,
                                            const beart::Sampler &sampler,
                                            beart::Spectrum *normal,
                                            beart::Spectrum *albedos) const {

  Spectrum L(0.f, 0.f, 0.f);
  if (ray.depth_ > max_depth_) {
    return L;
  }

  // get the intersection between the ray and the scene
  SurfaceInterection info;
  if (!scene.Intersect(ray, &info)) {
    return scene.Le(ray);
  }
  if (normal) {
    *normal += (info.Ns + 1.f) * 0.5f;
  }
  // accumulate the light emitted by intersection itself
  L += info.Le(-ray.dir_);
  if (info.primitive->bxdfs()->empty()) {
    return L;
  }

  Event event(info);
  // evaluate each direct light (may add sampling based on the Power)
  auto lights = scene.lights();
  for (const auto &light : lights) {
    const auto light_sample = LightSample(sampler.Next2D());
    const auto bsdf_sample = BsdfSample(sampler.Next2D());
    L += DirectIllumination(event, ray, scene, *light, light_sample, bsdf_sample);
  }
  return L;
}
