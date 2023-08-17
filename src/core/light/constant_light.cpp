//
// Created by Bear on 2023/8/16.
//

#include "constant_light.h"
#include "sample_common.h"
#include "event.h"

#include <spdlog/spdlog.h>
beart::Spectrum beart::ConstantLight::SampleLi(const beart::SurfaceInterection &info,
                                               const beart::LightSample &ls,
                                               beart::Vec3f *wi,
                                               float *pdf_solid,
                                               float *emission_pdf,
                                               float *distance,
                                               float *cos_light,
                                               beart::Visibility *visibility) const {
  Event event(info);
  *wi = SampleCosineHemiSphere(ls.u_, ls.v_);
//  Vec3f light_normal;
  // get one point on the light area
//  Point3f p_light = shape_->SampleDirect(ls, info.intersect_pos, *wi, &light_normal, pdf_solid);

//  float dist = Norm(p_light - info.intersect_pos);
  if (pdf_solid && *pdf_solid == 0.f) {
    return {0.f};
  }
  if (pdf_solid) {
    *pdf_solid = 1.f;
  }
  if (emission_pdf) {
    *emission_pdf = SampleCosineHemiSpherePdf(*wi);
//    *emission_pdf = SampleUniformSpherePdf() / shape_->SurfaceArea();
  }
  *wi = event.Local2World(*wi); // transform to world space from local space
  if (distance) {
    *distance = scene_->world_aabb().MaxAxisExtents();
//    *distance = dist;
  }
  if (cos_light) {
//    *cos_light = SafeDot(-*wi, light_normal);
    *cos_light = AbsDot(Normalize(info.intersect_pos - shape_->center()), *wi);
  }
  visibility->ray_ = Ray(info.intersect_pos, *wi, 1, false, kEpsilon);
  return radiance_;
}
beart::Spectrum beart::ConstantLight::SampleLi(const beart::LightSample &ls_pos,
                                               const beart::LightSample &ls_dir,
                                               beart::Ray &ray,
                                               float *pdf_solid,
                                               float *pdf_area,
                                               float *cos_light) const {
  return radiance_;
}
beart::Spectrum beart::ConstantLight::Power() const {
  return shape_->SurfaceArea() * radiance_ * kPi;
}
void beart::ConstantLight::set_scene(const beart::Scene *scene) {
  Light::set_scene(scene);
  if (!shape_) {
    auto center = scene_->world_aabb().Centroid();
    auto length = Norm(center - scene_->world_aabb()[1]) * 1.1;
    shape_ = std::make_unique<Sphere>(center, length);
  }
}
const beart::Shape *beart::ConstantLight::shape() const {
  return shape_.get();
}
beart::Spectrum beart::ConstantLight::Le(const beart::SurfaceInterection &info,
                                         const beart::Vec3f &wo,
                                         float *pdf_area,
                                         float *pdf_solid) const {
  if (pdf_area) {
    *pdf_area = 1.f / shape_->SurfaceArea(); // based on surface area
  }
  if (pdf_solid) {
    *pdf_solid = SampleUniformSpherePdf() / shape_->SurfaceArea(); // based on solid angle
  }
  return radiance_;
}
bool beart::ConstantLight::Le(const beart::Ray &ray, beart::SurfaceInterection *info, beart::Spectrum *radiance) const {
  if (!shape_->Intersect(ray, info)) {
    return false;
  }
  *radiance = Le(*info, -ray.dir_, nullptr, nullptr); // ray.dir = wi
  return true;
}
BEART_REGISTER_CLASS_IN_FACTORY(Light, ConstantLight, "constant")

