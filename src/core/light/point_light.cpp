//
// Created by Bear on 2023/07/09.
//

#include "samples.h"
#include "point_light.h"
#include "sample_common.h"
#include "factory.h"

beart::Spectrum beart::PointLight::SampleLi(const beart::SurfaceInterection &info,
                                            const beart::LightSample &ls,
                                            beart::Vec3f *wi,
                                            float *pdf_solid,
                                            float *emission_pdf,
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
  visibility->ray_ = Ray(info.intersect_pos, *wi, 1, false, kEpsilon, dist - kEpsilon);
  if (pdf_solid) {  // PDF of picking sampling direction based solid angle
    *pdf_solid = 1.;
  }
  if (emission_pdf) {
    *emission_pdf = SampleUniformSpherePdf();
  }
  if (distance) {
    *distance = dist;
  }
  if (cos_light) {
    *cos_light = 1.0;
  }
  return intensity_ * (inv_dist * inv_dist);
}
beart::Spectrum beart::PointLight::SampleLi(const LightSample &ls_pos,
                                            const LightSample &ls_dir,
                                            beart::Ray &ray,
                                            float *pdf_solid,
                                            float *pdf_area,
                                            float *cos_light) const {
  auto light_pos = pos_;
  // re-call constructor
  ray = Ray(light_pos, SampleUniformSphere(ls_dir.u_, ls_dir.v_), ray.depth_, ray.is_primary_ray_);
  if (pdf_solid) {
    *pdf_solid = SampleUniformSpherePdf();
  }
  if (pdf_area) {
    *pdf_area = 1.0f;
  }
  if (cos_light) {
    *cos_light = 1.0f;
  }
  return intensity_;
}
BEART_REGISTER_CLASS_IN_FACTORY(Light, PointLight, "point")
