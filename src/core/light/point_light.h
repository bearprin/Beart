//
// Created by Bear on 2022/4/10.
//

#pragma once

#include "common.h"
#include "light.h"
namespace beart {
class PointLight : public Light {
 public:
  PointLight(const Vec3f &pos, const Spectrum &intensity) : Light(Translate(pos), intensity) {
  }
  Spectrum SampleLi(const IntersectionInfo &info,
                    const LightSample &ls,
                    Vec3f *wi,
                    float *pdf_s,
                    float *distance,
                    float *cos_light,
                    Visibility *visibility) const override;
  [[nodiscard]] bool IsDelta() const override {
    return true;
  }
  Spectrum Power() override {
    return 4 * kPi * intensity_;
  }

};
}
