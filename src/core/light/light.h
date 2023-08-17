//
// Created by Bear on 2023/07/09.
//

#pragma once

#include "scene.h"

namespace beart {
enum class LightType {
  kPoint,
  kDirectional,
  kSpot,
  kArea,
  kInfinite,
  kConstant,
  kUnknown
};
class Light {
 public:
  Light(LightType light_type, const Transform &light_to_world = Transform::Identity())
      : light_type_(light_type), light_to_world_(light_to_world), world_to_light_(Inverse(light_to_world)) {}
  Light(const json &j) : sample_weight_(j.value("sample_weight", 1.0)) {}

  virtual ~Light() = default;
  virtual bool IsDelta() const {
    return true;
  }
  virtual bool IsInfinite() const {
    return true;
  }
  virtual void set_scene(const Scene *scene) {
    scene_ = scene;
  }
  virtual const Shape *shape() const {
    return nullptr;
  }
  /// \brief Compute the radiance that from light to intersection point. Resulting 0 for delta lights naturally.
  /// \param info
  /// \param wo
  /// \return
  virtual Spectrum Le(const SurfaceInterection &info, const Vec3f &wo, float *pdf_area, float *pdf_solid) const {
    return Spectrum{0.f, 0.f, 0.f};
  }
  virtual bool Le(const Ray &ray, SurfaceInterection *info, Spectrum *radiance) const {
    return false;
  }

  /// Sampling point and compute the radiance arriving at the point on the surface
  /// return pdf simultaneously
  /// \param info  The intersection info
  /// \param ls  The light sample with random number
  /// \param wi  The sampling direction of light
  /// \param pdf_s The PDF of picking sampling direction based solid angle
  /// \param emission_pdf The PDF of picking sampling direction and position based solid angle
  /// \param distance  The distance between the light and the intersection point
  /// \param cos_light The cosine of the angle between the sampling light direction and the light
  /// \param visibility
  /// \return
  virtual Spectrum SampleLi(const SurfaceInterection &info,
                            const LightSample &ls,
                            Vec3f *wi,
                            float *pdf_solid,
                            float *emission_pdf,
                            float *distance,
                            float *cos_light,
                            Visibility *visibility) const = 0;
  /// Sampling outgoing ray from light
  /// \param ls
  /// \param ray
  /// \param pdf_solid
  /// \param pdf_area
  /// \param cos_light
  /// \return
  virtual Spectrum SampleLi(const LightSample &ls_pos, const LightSample &ls_dir,
                            Ray &ray,
                            float *pdf_solid,
                            float *pdf_area,
                            float *cos_light) const = 0;
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

  LightType light_type() const {
    return light_type_;
  }

 protected:
  const Scene *scene_ = nullptr;
  Transform light_to_world_;
  Transform world_to_light_;

  LightType light_type_ = LightType::kUnknown;
  float sample_weight_ = 1.0;
};
}
