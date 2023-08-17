//
// Created by Bear on 2023/8/16.
//

#pragma once
#include "light.h"
#include "sphere.h"

#include <memory>
namespace beart {
// Assuming there is a very large sphere light out of the world
class ConstantLight : public Light {
 public:
  ConstantLight() : Light(LightType::kConstant) {}
  ConstantLight(const json &j) : Light(LightType::kConstant) {
    radiance_ = j.value("radiance", Spectrum{2.f});
  }
  Spectrum SampleLi(const SurfaceInterection &info,
                    const LightSample &ls,
                    Vec3f *wi,
                    float *pdf_solid,
                    float *emission_pdf,
                    float *distance,
                    float *cos_light,
                    Visibility *visibility) const override;
  Spectrum SampleLi(const LightSample &ls_pos,
                    const LightSample &ls_dir,
                    Ray &ray,
                    float *pdf_solid,
                    float *pdf_area,
                    float *cos_light) const override;
  Spectrum Power() const override;
  void set_scene(const Scene *scene) override;
  const Shape *shape() const override;
  Spectrum Le(const SurfaceInterection &info, const Vec3f &wo, float *pdf_area, float *pdf_solid) const override;
  bool Le(const Ray &ray, SurfaceInterection *info, Spectrum *radiance) const override;
  bool IsDelta() const override {
    return false;
  }
  bool IsInfinite() const override {
    return false;
  }
 private:
  Spectrum radiance_;
  std::unique_ptr<Sphere> shape_ = nullptr;

};
}
