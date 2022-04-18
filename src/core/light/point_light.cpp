//
// Created by Bear on 2022/4/13.
//

#include "intersection_info.h"
#include "samples.h"
#include "transform.h"
#include "vec.h"
#include "point_light.h"
#include "spectrum.h"

beart::Spectrum beart::PointLight::SampleLi(const IntersectionInfo &info,
                                            const LightSample &ls,
                                            Vec3f *wi,
                                            float *pdf,
                                            float *distance,
                                            Visibility *visibility) const {
  // transform light point from light space to world space
  auto light_pos = TransformPoint(light_to_world_, Vec3f{0., 0., 0.});
  // ray from light to point
  auto pos_to_light_dir = light_pos - info.corrds;
  auto dist = pos_to_light_dir.norm();
  auto sq_dist = pos_to_light_dir.norm() * pos_to_light_dir.norm();
  // init visibility test
  auto eps = 0.01f;
  visibility->ray_ = Ray(info.corrds, Normalize(pos_to_light_dir), 0, eps, dist);

  if (wi) {
    *wi = Normalize(pos_to_light_dir);
  }
  if (distance) {
    *distance = dist;
  }
  if (pdf) {
    *pdf = 1.f;
  }
  // return light intensity
  return intensity_ / sq_dist;
}

