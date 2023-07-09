//
// Created by Bear on 2023/07/09.
//

#pragma once

#include "light.h"
namespace beart {
class PointLight : public Light {
 public:
  PointLight(const Vec3f &pos, const Spectrum &intensity) : Light(intensity, Translate(pos)) {
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
  Spectrum Power() const override {
    return 4 * kPi * intensity_;
  }

};
}
