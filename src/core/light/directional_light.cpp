//
// Created by Bear on 2022/4/19.
//

#include "directional_light.h"
beart::Spectrum beart::DirectionalLight::SampleLi(const beart::IntersectionInfo &info,
                                                  const beart::LightSample &ls,
                                                  beart::Vec3f *wi,
                                                  float *pdf_s,
                                                  float *distance,
                                                  float *cos_light,
                                                  beart::Visibility *visibility) const {
  Vec3f light_dir = direction_;
  *wi = -light_dir;
  // init visibility test: from intersection point to light, add bias to avoid self-intersection
  auto eps = 0.01f;
  visibility->ray_ = Ray(info.corrds, Normalize(*wi), 0, eps);

  if (pdf_s) {
    *pdf_s = 1.f;
  }
  if (distance) {
    *distance = 1e6;  // approximate infinite distance
  }
  if (cos_light) {
    *cos_light = 1.f;
  }
  return intensity_;
}
