//
// Created by Bear on 2023/7/12.
//

#pragma once

#include "light.h"

#include <utility>
namespace beart {
class DirectionalLight : public beart::Light {
 public:
  DirectionalLight(Vec3f direction, Spectrum irradiance)
      : direction_(std::move(direction)), irradiance_(std::move(irradiance)), Light(LightType::kDirectional) {

  }
  DirectionalLight(const json &json) : DirectionalLight(json.value("dir", Vec3f{0.f, 0.f, 1.f}),
                                                        json.value("irradiance", Spectrum{1.f})) {

  }

  Spectrum SampleLi(const SurfaceInterection &info,
                    const LightSample &ls,
                    Vec3f *wi,
                    float *pdf_solid,
                    float *emission_pdf,
                    float *distance,
                    float *cos_light,
                    Visibility *visibility) const override;
  Spectrum SampleLi(const LightSample &ls_pos, const LightSample &ls_dir,
                    Ray &ray,
                    float *pdf_solid,
                    float *pdf_area,
                    float *cos_light) const override;
  bool IsDelta() const override {
    return true;
  }
  bool IsInfinite() const override {
    return true;
  }
  // approximate power for directional light
  Spectrum Power() const override {
    auto aabb = this->scene_->world_aabb();
    float delta = Norm(aabb[1] - aabb[0]);
    return delta * kPi * irradiance_;
  }

 private:
  Vec3f direction_; // note that this is the direction from light to the scene
  Spectrum irradiance_;
};
}