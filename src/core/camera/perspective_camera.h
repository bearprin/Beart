//
// Created by Bear on 2023/07/6.
//

#pragma once

#include "camera.h"
#include "interection.h"
#include "sampler.h"
#include "factory.h"
namespace beart {
class PerspectiveCamera : public Camera {
 public:
  PerspectiveCamera(unsigned int image_width,
                    unsigned int image_height,
                    const Vec3f &target = Vec3f{0.f, 0.f, -1.0f},
                    const Vec3f &camera_pos = Vec3f{0.f, 0.f, 0.f},
                    const Vec3f &up = Vec3f{0.f, 1.f, 0.0f},
                    float fov = 30.f,
                    float near_clip = 0.01f,
                    float far_clip = 10000.f);
  PerspectiveCamera(const json &j) : PerspectiveCamera(j.value("image_width", 512),
                                                       j.value("image_height", 512),
                                                       j.value("target", Vec3f{0.f, 0.f, -1.0f}),
                                                       j.value("camera_pos", Vec3f{0.f, 0.f, 0.f}),
                                                       j.value("up", Vec3f{0.f, 1.f, 0.0f}),
                                                       j.value("fov", 30.f),
                                                       j.value("near_clip", 0.01f),
                                                       j.value("far_clip", 10000.f)) {}

  [[nodiscard]] Ray GenerateRay(const float &x,
                                const float &y,
                                const PixelSample &pixel_sample) const noexcept override;
  Vec2i GetScreenCoord(const SurfaceInterection &inter,
                       beart::Visibility &visibility,
                       const Sampler &sampler,
                       float &cos_camera,
                       float *pdf_solid,
                       float *pdf_area,
                       beart::Spectrum *we,
                       beart::Point3f *eye_p) const noexcept override;
 private:
  float fov_;
  float near_clip_;
  float far_clip_;
  float image_aspect_;
  float image_plane_dist_ = 0.0f;  // Distance to the image plane z=1


  Transform raster_to_screen_;
  Transform raster_to_world_;
  Transform raster_to_camera_;

  Transform screen_to_raster_;

  Transform camera_to_screen_;
  Transform camera_to_raster_;

  Transform world_to_raster_;

  // TODO: add DOF effect
  float lens_radius_ = 0.0f;            /**< Radius of the camera lens. */
  float focal_distance_ = 0.0f;         /**< The focal distance for DOF effect. */
  float inverse_aparture_size_ = 0.0f;   /**< Reciprocal of the aperture size. */
};
}