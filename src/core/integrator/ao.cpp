//
// Created by Bear on 2023/07/10.
//

#include "ao.h"
#include "sample_common.h"
#include "event.h"
#include "factory.h"
beart::Spectrum beart::AOIntegrator::Li(const beart::Ray &ray,
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
//    return scene.Le(ray);
    return {0.};  // no intersection, return black for AO
  }
  if (normal) {
    // map the normal to [0, 1]
    *normal += (info.Ns + 1.f) * 0.5f;
  }
//  info.Ns = Dot(info.Ns, ray.dir_) >= 0.f ? -info.Ns : info.Ns; // make sure the normal is in the same side of the ray
  Event event(info);
  // sample the hemisphere with cosine importance sampling
  Vec3f wi = SampleCosineHemiSphere(sampler.Next1D(), sampler.Next1D());
  float pdf = SampleCosineHemiSpherePdf(wi);
  wi = event.Local2World(wi); // transform to world space from local space

  const float d = Dot(wi, info.Ns);
  if (d <= 0.0f) {  //
    return {0.0f};
  }

  // the ray to be tested
  Ray shadow_ray(info.intersect_pos, wi, 1, false, kEpsilon, max_distance - kEpsilon);
  SurfaceInterection ao_test_info;
  if (!scene.IsOccluded(shadow_ray)) {  // no intersection,
    L = d * kInvPi / pdf; // lambertian materrail / pdf
    return L;
  }
  return {0.f};
}
BEART_REGISTER_CLASS_IN_FACTORY(Integrator, AOIntegrator, "ao")

