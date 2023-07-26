//
// Created by Bear on 2023/7/14.
//

#include "area_light.h"
#include "sample_common.h"
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
                                           float *pdf_s,
                                           float *distance,
                                           float *cos_light,
                                           beart::Visibility *visibility) const {
  Vec3f light_normal;
  // get one point on the light area
  Point3f p_light = shape_->SampleDirect(ls, info.intersect_pos, *wi, &light_normal, pdf_s);

  float dist = Norm(p_light - info.intersect_pos);
  if (pdf_s && *pdf_s == 0.f) {
    return {0.f};
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
