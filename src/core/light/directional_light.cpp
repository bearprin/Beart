//
// Created by Bear on 2023/7/12.
//

#include "directional_light.h"

beart::Spectrum beart::DirectionalLight::SampleLi(const beart::SurfaceInterection &info,
                                                  const beart::LightSample &ls,
                                                  beart::Vec3f *wi,
                                                  float *pdf_solid,
                                                  float *distance,
                                                  float *cos_light,
                                                  beart::Visibility *visibility) const {

  // distant light direction
  auto dir_to_light = -direction_;
  if (wi) {
    *wi = dir_to_light;
  }

  if (pdf_solid) {
    *pdf_solid = 1.0f;
  }

  if (distance) {
    *distance = 1e6f; // mark as infinity
  }

  if (cos_light) {
    *cos_light = 1.0f;
  }
  visibility->ray_ = Ray(info.intersect_pos, *wi, 0, false, kEpsilon);

  return irradiance_;
}
