//
// Created by Bear on 2023/7/12.
//

#include "directional_light.h"
#include "sample_common.h"
#include "factory.h"
beart::Spectrum beart::DirectionalLight::SampleLi(const beart::SurfaceInterection &info,
                                                  const beart::LightSample &ls,
                                                  beart::Vec3f *wi,
                                                  float *pdf_solid,
                                                  float *emission_pdf,
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
  if (emission_pdf) {
    auto [b_min, b_max] = scene_->world_aabb().Bounds();
    auto delta = b_max - b_min;
    *emission_pdf = 4.f * kInvPi / (SquareNorm(delta));
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
beart::Spectrum beart::DirectionalLight::SampleLi(
    const LightSample &ls_pos, const LightSample &ls_dir,
    beart::Ray &ray,
    float *pdf_solid,
    float *pdf_area,
    float *cos_light) const {
  // sample center of the scene
  auto world_bbox = this->scene_->world_aabb();
  auto center = world_bbox.Centroid();
  auto world_radius = Norm(world_bbox.Bounds().second - center);

  Vec3f v0;
  Vec3f v1;
  CoordinateSystem(direction_, &v0, &v1);
  auto [u, v] = SampleUniformDisk(ls_pos.u_, ls_pos.v_);
  Point3f p = center + world_radius * (u * v0 + v * v1);
  // offset p to the outside of the scene, 3 is empirical
  auto offset = direction_ * world_radius * 3.0;
  ray = Ray(p + offset, direction_, ray.depth_, ray.is_primary_ray_, 0, kInfinity);
  float pdf = 1.f / (kPi * world_radius * world_radius);
  if (pdf_solid) {
    *pdf_solid = pdf;
  }
  if (pdf_area) {
    *pdf_area = 1.0f;
  }
  if (cos_light) {
    *cos_light = 1.0f; // for delta light
  }
  return irradiance_;
}
BEART_REGISTER_CLASS_IN_FACTORY(Light, DirectionalLight, "directional")

