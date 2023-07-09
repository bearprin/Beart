//
// Created by Bear on 2023/07/09.
//

#pragma once

#include "scene.h"

namespace beart {
class Light {
 public:
  Light(Spectrum intensity) : intensity_(std::move(intensity)) {}
  Light(Spectrum intensity, const Transform &light_to_world)
      : intensity_(std::move(intensity)), light_to_world_(light_to_world), world_to_light_(Inverse(light_to_world)) {}
  virtual ~Light() = default;
  [[nodiscard]] virtual bool IsDelta() const {
    return true;
  }
  [[nodiscard]] virtual bool IsInfinite() const {
    return false;
  }
  /// \brief Compute the radiance that from light to intersection point. Resulting 0 for delta lights naturally.
  /// \param info
  /// \param wo
  /// \return
  virtual Spectrum Le(const SurfaceInterection &info, const Vec3f &wo) const {
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
  virtual Spectrum SampleLi(const SurfaceInterection &info,
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
  virtual Spectrum Power() const = 0;

 protected:
  Spectrum intensity_;  // Radiant intensity (W / sr)
  Transform light_to_world_;
  Transform world_to_light_;

  float sample_weight_ = 1.0;
};
}
