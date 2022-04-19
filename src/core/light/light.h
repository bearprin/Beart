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
  Light(Spectrum intensity) : intensity_(std::move(intensity)) {}
  Light(const Transform &light_to_world, Spectrum intensity)
      : light_to_world_(light_to_world), intensity_(std::move(intensity)), world_to_light_(Inverse(light_to_world)) {}
  ~Light() = default;
  [[nodiscard]] virtual bool IsDelta() const {
    return false;
  }
  [[nodiscard]] virtual bool IsInfinite() const {
    return false;
  }
  /// \brief Compute the radiance that from light to intersection point. Resulting 0 for delta lights naturally.
  /// \param info
  /// \param wo
  /// \return
  virtual Spectrum Le(const IntersectionInfo &info, const Vec3f &wo) const {
    return Spectrum{0.f, 0.f, 0.f};
  }

  /// \brief Sampling point and compute the radiance arriving at the point on the surface
  /// return pdf simultaneously
  /// \param info  The intersection info
  /// \param ls  The light sample with random number
  /// \param wi  The sampling direction of light
  /// \param pdf_s The PDF of picking sampling direction based solid angle
  /// \param distance  The distance between the light and the intersection point
  /// \param cos_light The cosine of the angle between the sampling light direction and the light
  /// \param visibility
  /// \return
  virtual Spectrum SampleLi(const IntersectionInfo &info,
                            const LightSample &ls,
                            Vec3f *wi,
                            float *pdf_s,
                            float *distance,
                            float *cos_light,
                            Visibility *visibility) const = 0;
  /// \brief Compute the pdf of sampling the light
  /// \param p ray origin
  /// \param wi ray direction
  /// \return
  virtual float Pdf(const Vec3f &p, const Vec3f &wi) const {
    return 1.0;
  }
  /// \brief Integral over sphere to get the power (W)
  /// \return the power of the light
  virtual Spectrum Power() = 0;

 protected:
  Transform light_to_world_;
  Spectrum intensity_;  // Radiant intensity (W / sr)
  Transform world_to_light_;
  float sample_weight_ = 1.0;
};
}
