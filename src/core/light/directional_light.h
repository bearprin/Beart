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
      : direction_(std::move(direction)), irradiance_(std::move(irradiance)) {

  }

  Spectrum SampleLi(const SurfaceInterection &info,
                    const LightSample &ls,
                    Vec3f *wi,
                    float *pdf_s,
                    float *distance,
                    float *cos_light,
                    Visibility *visibility) const override;
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