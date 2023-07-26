//
// Created by Bear on 2023/7/10.
//
#include "scene.h"
#include "spectrum.h"
#include "light.h"
#include "event.h"

namespace beart {
static float Mis(float f, float g) {
  auto weight = f * f / (f * f + g * g);
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
  // light sampling based solid angle
  auto li = light.SampleLi(event.info(), ls, &wi, &light_pdf, &distance, &cos_light, &vis_test);
  if (light_pdf > 0.f && !IsBlack(li) && cos_light > 0.f) {
    // evaluate BSDF * cos(theta)
    auto f = event.EvaluateBxDF(wo, wi);
    if (!IsBlack(f) && vis_test.IsVisible()) { // intersection point is visible to light
      if (light.IsDelta()) {
        radiance += li * f / light_pdf;
      } else {  // Non-delta light, sampling the area of the light
        bsdf_pdf = event.Pdf(wo, wi);
        radiance += li * f * Mis(light_pdf, bsdf_pdf) / light_pdf;
      }
    }
  }
  // bsdf sampling
  if (!light.IsDelta()) {
    const auto f = event.SampleF(wo, wi, bs, &bsdf_pdf);  // sample bsdf get wi and pdf
    if (!IsBlack(f) && bsdf_pdf != 0.f) {
      light_pdf = light.Pdf(event.info().intersect_pos, wi);
      if (light_pdf <= 0.f) {
        return radiance;
      }
      auto shadow_ray = Ray{event.info().intersect_pos, wi, ray.depth_ + 1, false, kEpsilon};
      SurfaceInterection shadow_info;
      Spectrum li;
      if (!light.Le(shadow_ray, &shadow_info, &li)) {  // find intersection of the light
        return radiance;
      }
      vis_test.ray_ =
          Ray{event.info().intersect_pos, wi, ray.depth_ + 1, false, kEpsilon, shadow_info.t_curr - kEpsilon};
      if (!IsBlack(li) && vis_test.IsVisible()) {
        radiance += li * f * Mis(bsdf_pdf, light_pdf) / bsdf_pdf;
//        radiance += li * f / bsdf_pdf;
      }
    }
  }
  return radiance;
}
}