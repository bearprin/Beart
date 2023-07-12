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
      : Light(Translate(pos)), pos_(std::move(pos)), intensity_(std::move(intensity)) {

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
 private:
  Point3f pos_;
  Vec3f intensity_;

};
}
