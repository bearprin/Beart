//
// Created by Bear on 2022/4/19.
//

#pragma once

#include "light.h"

#include <utility>
namespace beart {
class DirectionalLight : public Light {
 public:
  DirectionalLight(const Vec3f &direction, const Spectrum &intensity) : Light(intensity),
                                                                        direction_(Normalize(direction)) {
    // TODO: Init light2world_
  }

  [[nodiscard]] bool IsDelta() const override {
    return true;
  }
  [[nodiscard]] bool IsInfinite() const override {
    return true;
  }
  Spectrum Power() override {

  }
  Spectrum SampleLi(const IntersectionInfo &info,
                    const LightSample &ls,
                    Vec3f *wi,
                    float *pdf_s,
                    float *distance,
                    float *cos_light,
                    Visibility *visibility) const override;
 private:
  Vec3f direction_;
};
}
