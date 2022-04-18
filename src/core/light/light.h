//
// Created by Bear on 2021/12/3.
//

#pragma once

#include <utility>

#include "spectrum.h"
#include "intersection_info.h"
#include "scene.h"

namespace beart {
class Visibility {
 public:
  explicit Visibility(const Scene &scene) : scene_(scene) {}
  [[nodiscard]] bool IsVisible() const {
    return !scene_.IsOccluded(ray_);
  }
  Ray ray_;
 private:
  const Scene &scene_;
};
class Light {
 public:
  Light(const Transform &light_to_world, Spectrum intensity)
      : light_to_world_(light_to_world), intensity_(std::move(intensity)), world_to_light_(Inverse(light_to_world)) {}
  ~Light() = default;
  virtual bool IsDelta() const {
    return false;
  }
  ///
  /// \param info
  /// \param wo
  /// \return
  virtual Spectrum Le(const IntersectionInfo &info, const Vec3f &wo) const {
    return Spectrum{0.f, 0.f, 0.f};
  }

  /// \brief Sampling point and compute the radiance arriving at the point on the surface
  /// \param info
  /// \param ls
  /// \param wi
  /// \param pdf
  /// \param distance
  /// \param visibility
  /// \return
  virtual Spectrum SampleLi(const IntersectionInfo &info,
                            const LightSample &ls,
                            Vec3f *wi,
                            float *pdf,
                            float *distance,
                            Visibility *visibility) const = 0;
  /// \brief
  /// \return
  virtual Spectrum power() = 0;
 protected:
  Transform light_to_world_;
  Spectrum intensity_;
  Transform world_to_light_;
  float sample_weight_ = 1.0;
};
}
