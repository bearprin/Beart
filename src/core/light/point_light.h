//
// Created by Bear on 2023/07/09.
//

#pragma once

#include "light.h"

#include <utility>
namespace beart {
class PointLight : public Light {
 public:
  PointLight(Point3f pos, Spectrum intensity)
      : Light(LightType::kPoint, Translate(pos)), pos_(std::move(pos)), intensity_(std::move(intensity)) {

  }
  PointLight(const json &j) : PointLight(j.value("pos", Point3f{0.f}),
                                         j.value("intensity", Vec3f{1.0f})) {

  }
  Spectrum SampleLi(const SurfaceInterection &info,
                    const LightSample &ls,
                    Vec3f *wi,
                    float *pdf_solid,
                    float *emission_pdf,
                    float *distance,
                    float *cos_light,
                    Visibility *visibility) const override;
  virtual Spectrum SampleLi(const LightSample &ls_pos, const LightSample &ls_dir,
                            Ray &ray,
                            float *pdf_solid,
                            float *pdf_area,
                            float *cos_light) const override;
  bool IsDelta() const override {
    return true;
  }
  Spectrum Power() const override {
    return 4 * kPi * intensity_;
  }
 private:
  Point3f pos_;
  Vec3f intensity_;

};
}
