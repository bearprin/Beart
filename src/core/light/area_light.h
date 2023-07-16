//
// Created by Bear on 2023/7/14.
//

#pragma once

#include "light.h"
namespace beart {
class AreaLight : public Light {
 public:
  AreaLight(std::unique_ptr<Shape> shape, Spectrum radiance)
      : shape_(std::move(shape)), radiance_(std::move(radiance)), Light(shape->ObjToWorld()) {

  }
  const Shape *shape() const {
    return shape_.get();
  }
  bool IsDelta() const override {
    return false;
  }
  bool IsInfinite() const override {
    return false;
  }
  Spectrum Le(const SurfaceInterection &info, const Vec3f &wo, float *pdf_area, float *pdf_solid) const override;
  Spectrum SampleLi(const SurfaceInterection &info,
                    const LightSample &ls,
                    Vec3f *wi,
                    float *pdf_solid,
                    float *distance,
                    float *cos_light,
                    Visibility *visibility) const override;
  float Pdf(const Vec3f &p, const Vec3f &wi) const override;
  Spectrum Power() const override;

 private:
  std::unique_ptr<Shape> shape_ = nullptr;
  Spectrum radiance_;

};
}
