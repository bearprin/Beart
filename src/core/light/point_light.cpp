//
// Created by Bear on 2023/07/09.
//

#include "samples.h"
#include "point_light.h"

beart::Spectrum beart::PointLight::SampleLi(const beart::SurfaceInterection &info,
                                            const beart::LightSample &ls,
                                            beart::Vec3f *wi,
                                            float *pdf_s,
                                            float *distance,
                                            float *cos_light,
                                            beart::Visibility *visibility) const {
  auto light_pos = pos_;
  // ray from light to point
  Vec3f pos_to_light_dir = light_pos - info.intersect_pos;
  *wi = Normalize(pos_to_light_dir);
  float dist = Norm(pos_to_light_dir);
  float inv_dist = 1.0f / dist;
  // init visibility test: from intersection point to light, add bias to avoid self-intersection
//  auto eps = 0.01f;
  visibility->ray_ = Ray(info.intersect_pos, *wi, 0, false, kEpsilon, dist);
  if (pdf_s) {  // PDF of picking sampling direction based solid angle
    *pdf_s = 1.;
  }
  if (distance) {
    *distance = dist;
  }
  if (cos_light) {
    *cos_light = 1.0;
  }
  return intensity_ * (inv_dist * inv_dist);
}
