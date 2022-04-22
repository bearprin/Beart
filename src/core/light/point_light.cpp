//
// Created by Bear on 2022/4/13.
//

#include "intersection_info.h"
#include "samples.h"
#include "transform.h"
#include "vec.h"
#include "point_light.h"
#include "spectrum.h"

beart::Spectrum beart::PointLight::SampleLi(const beart::IntersectionInfo &info,
                                            const beart::LightSample &ls,
                                            beart::Vec3f *wi,
                                            float *pdf_s,
                                            float *distance,
                                            float *cos_light,
                                            beart::Visibility *visibility) const {
  // transform light point from light space to world space
  auto light_pos = TransformPoint(light_to_world_, Vec3f{0., 0., 0.});
  // ray from light to point
  auto pos_to_light_dir = light_pos - info.corrds;
  *wi = Normalize(pos_to_light_dir);
  auto dist = pos_to_light_dir.norm();
  auto sq_dist = pos_to_light_dir.norm() * pos_to_light_dir.norm();
  // init visibility test: from intersection point to light, add bias to avoid self-intersection
  auto eps = 0.01f;
  visibility->ray_ = Ray(info.corrds, *wi, 0, eps, dist);
  if (pdf_s) {  // PDF of picking sampling direction based solid angle
    *pdf_s = 1.;
  }
  if (distance) {
    *distance = dist;
  }
  if (cos_light) {
    *cos_light = 1.0;
  }
  return intensity_;
}
