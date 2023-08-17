//
// Created by Bear on 2023/7/14.
//

#include "area_light.h"
#include "sample_common.h"
#include "factory.h"
beart::AreaLight::AreaLight(const beart::json &j) : AreaLight(BeartFactory<Shape>::CreateUnique(j["shape"]),
                                                              j.value("radiance", Spectrum{1.f})) {

}
beart::Spectrum beart::AreaLight::Le(const beart::SurfaceInterection &info,
                                     const beart::Vec3f &wo,
                                     float *pdf_area,
                                     float *pdf_solid) const {
  float cos = SafeDot(info.Ng, wo);
  if (cos <= 0.f) { //
    return Spectrum{0.f, 0.f, 0.f};
  }
  if (pdf_area) {
    *pdf_area = 1.f / shape_->SurfaceArea(); // based on surface area
  }
  if (pdf_solid) {
    *pdf_solid = SampleUniformSpherePdf() / shape_->SurfaceArea(); // based on solid angle
  }
  return radiance_;
}
bool beart::AreaLight::Le(const beart::Ray &ray, beart::SurfaceInterection *info, beart::Spectrum *radiance) const {
  if (!shape_->Intersect(ray, info)) {
    return false;
  }
  *radiance = Le(*info, -ray.dir_, nullptr, nullptr); // ray.dir = wi
  return true;
}

beart::Spectrum beart::AreaLight::SampleLi(const beart::SurfaceInterection &info,
                                           const beart::LightSample &ls,
                                           beart::Vec3f *wi,
                                           float *pdf_solid,
                                           float *emission_pdf,
                                           float *distance,
                                           float *cos_light,
                                           beart::Visibility *visibility) const {
  Vec3f light_normal;
  // get one point on the light area
  Point3f p_light = shape_->SampleDirect(ls, info.intersect_pos, *wi, &light_normal, pdf_solid);

  float dist = Norm(p_light - info.intersect_pos);
  if (pdf_solid && *pdf_solid == 0.f) {
    return {0.f};
  }
  if (emission_pdf) {
    *emission_pdf = SampleUniformSpherePdf() / shape_->SurfaceArea();
  }
  if (distance) {
    *distance = dist;
  }
  if (cos_light) {
    *cos_light = SafeDot(-*wi, light_normal);
  }
  visibility->ray_ = Ray{info.intersect_pos, *wi, 1, false, kEpsilon, dist - kEpsilon};
  return radiance_;
}
float beart::AreaLight::Pdf(const beart::Vec3f &p, const beart::Vec3f &wi) const {
  return shape_->DirectPdf(p, wi);
}
beart::Spectrum beart::AreaLight::Power() const {
  return shape_->SurfaceArea() * radiance_ * kTwoPi;
}
beart::Spectrum beart::AreaLight::SampleLi(const LightSample &ls_pos, const LightSample &ls_dir,
                                           beart::Ray &ray,
                                           float *pdf_solid,
                                           float *pdf_area,
                                           float *cos_light) const {
  Vec3f normal;
  shape_->SampleDirect(ls_pos, ls_dir, ray, normal, pdf_solid);
  if (pdf_area) {
    *pdf_area = 1.0f / shape_->SurfaceArea();
  }
  if (cos_light) {
    *cos_light = SafeDot(ray.dir_, normal);
  }
  return radiance_;
}

BEART_REGISTER_CLASS_IN_FACTORY(Light, AreaLight, "area")
