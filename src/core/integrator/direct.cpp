//
// Created by Bear on 2022/4/14.
//

#include <iostream>
#include "direct.h"
#include "intersection_info.h"
#include "integrator_common.h"
beart::Spectrum beart::Direct::Li(const beart::Ray &ray,
                                  const beart::Scene &scene,
                                  const beart::PixelSample &sampler) const {
  Spectrum L(0.f, 0.f, 0.f);
  if (ray.depth() > max_depth_) {
    return L;
  }

  // get the intersection between the ray and the scene
  IntersectionInfo info;
  if (!scene.IntersectInfo(ray, &info)) {
    return scene.Le(ray);
  }
  // accumulate the light emitted by intersection itself
  L += info.Le(-ray.dir());

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
