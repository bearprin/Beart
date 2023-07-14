//
// Created by Bear on 2023/7/10.
//

#include "normal.h"
beart::Spectrum beart::NormalIntegrator::Li(const beart::Ray &ray,
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
  L = L + info.Le(-ray.dir_);

  // evaluate normal
  L = L + Spectrum{(info.Ng + 1.f) * 0.5f};
  return L;
}
