//
// Created by Bear on 2021/12/6.
//

#ifndef BEART_SRC_CORE_CAMERA_PERSPECTIVECAMERA_H_
#define BEART_SRC_CORE_CAMERA_PERSPECTIVECAMERA_H_

#include "Camera.h"
namespace beart {
class PerspectiveCamera : public Camera {
 public:
  PerspectiveCamera(unsigned int image_width,
                    unsigned int image_height,
                    const Vec3f &camera_pos = Vec3f{0.f, 0.f, 0.f},
                    const Vec3f &up = Vec3f{0.f, 1.f, 0.0f},
                    const Vec3f &dir = Vec3f{0.f, 0.f, -1.0f},
                    float near_clip = 0.01f,
                    float far_clip = 10000.f,
                    float fov = 0.25f);
  [[nodiscard]] Ray GenerateRay(const float &x,
                                const float &y,
                                const PixelSample &pixel_sample) const noexcept override;
 protected:
  float near_clip_;
  float far_clip_;
  float fov_;
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

#endif //BEART_SRC_CORE_CAMERA_PERSPECTIVECAMERA_H_
