//
// Created by Bear on 2023/7/10.
//
#include "scene.h"
#include "spectrum.h"
#include "light.h"
#include "event.h"

namespace beart {
static float Mis(float f, float g) {
  auto weight = f / (f + g);
  return weight;
}
static Spectrum DirectIllumination(const Event &event,
                                   const Ray &ray,
                                   const Scene &scene,
                                   const Light &light,
                                   const LightSample &ls,
                                   const BsdfSample &bs) {
  auto radiance = Spectrum{0.f, 0.f, 0.f};
  Visibility vis_test{scene};
  auto light_pdf = 0.f;
  auto bsdf_pdf = 0.f;
  auto distance{0.f};
  auto cos_light{0.f};
  Vec3f wo = -ray.dir_;
  Vec3f wi;
  auto li = light.SampleLi(event.info(), ls, &wi, &light_pdf, &distance, &cos_light, &vis_test);
  // light sampling
  if (light_pdf > 0.f) {
    // evaluate BSDF
    auto f = event.EvaluateBxDF(wo, wi);
    if (vis_test.IsVisible()) {
      if (light.IsDelta()) { // delta light not need light sampling
        radiance += li * f / light_pdf;
      }
    } else {  // light sampling with MIS
    }
  }
  return radiance;
}
}