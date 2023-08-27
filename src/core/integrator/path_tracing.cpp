//
// Created by Bear on 2023/7/27.
//

#include "path_tracing.h"
#include "integrator_common.h"
#include "factory.h"

beart::Spectrum beart::PathTracing::Li(const beart::Ray &ray,
                                       const beart::Scene &scene,
                                       const beart::Sampler &sampler,
                                       beart::Spectrum *normal,
                                       beart::Spectrum *albedos) const {
  Spectrum L(0.f, 0.f, 0.f);
  unsigned int local_bounce = 0;
  auto temp_r = ray;

  Spectrum throughput = {1.0f}; // define throughput

  while (true) {
    if (temp_r.depth_ > max_depth_) {
      return L;
    }
    // get the intersection between the ray and the scene
    SurfaceInterection info;
    if (!scene.Intersect(temp_r, &info)) {
      if (local_bounce == 0) { //
        return scene.Le(temp_r);
      }
      break; // no intersection
    }
    // accumulate the light emitted by intersection itself
    if (local_bounce == 0) {
      L += info.Le(-temp_r.dir_);
    }
    // record normal
    if (local_bounce == 0 && normal) {
      *normal += (info.Ns + 1.f) * 0.5f;
    }
    if (local_bounce == 0 && albedos) {
      *albedos += info.albedo;
    }
    if (info.primitive->bxdfs()->empty()) {
      return L;
    }
    Event event(info);

    // Next-event estimation: direct sampling
    float multi_light_pdf{};
    const auto light_sample = LightSample(sampler.Next2D(), sampler.Next1D());
    const auto bsdf_sample = BsdfSample(sampler.Next2D(), sampler.Next1D());
    auto light = scene.SampleLight(light_sample.t_, &multi_light_pdf); // sample multiple light sources
    if (multi_light_pdf > 0.f) {
      L += throughput * DirectIllumination(event, temp_r, scene, *light, light_sample, bsdf_sample) / multi_light_pdf;
    }

    // indirect sampling
    Vec3f wi;
    Vec3f wo = -temp_r.dir_;
    float bsdf_pdf{};
    const auto bs = BsdfSample(sampler.Next2D(), sampler.Next1D());
    auto f = event.SampleF(wo, wi, bs, &bsdf_pdf);  // sample bsdf get wi and pdf
    if (IsBlack(f) || bsdf_pdf <= 0.f) {
      break;
    }
    // update throughput (f * cos_theta / pdf), we merge cos_theta into f
    throughput *= f / bsdf_pdf;
    // update ray
    temp_r = Ray(info.intersect_pos, wi, temp_r.depth_ + 1, true, kEpsilon);

    if (local_bounce > 3) {
      // Russian Roulette
      float q = std::max(0.05f, 1.f - MaxComponent(f));
      if (sampler.Next1D() < q) {
        break;
      }
      throughput /= 1.f - q;
    }
    local_bounce++;
  }
  return L;
}
BEART_REGISTER_CLASS_IN_FACTORY(Integrator, PathTracing, "pt")
