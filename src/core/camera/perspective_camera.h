//
// Created by Bear on 2021/12/6.
//

#pragma once

#include "camera.h"
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
  [[nodiscard]] Ray GenerateRay(const float &x,
                                const float &y,
                                const PixelSample &pixel_sample) const noexcept override;
 protected:
  float fov_;
  float near_clip_;
  float far_clip_;
  float image_aspect_;

  Transform raster2screen_;
  Transform raster2_world_;
  Transform raster2camera_;

  Transform screen2raster_;

  Transform camera2screen_;
  Transform camera2raster_;

  Transform world2raster_;
};
}