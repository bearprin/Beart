//
// Created by Bear on 2021/12/6.
//

#include "perspective_camera.h"
beart::PerspectiveCamera::PerspectiveCamera(unsigned int image_width,
                                            unsigned int image_height,
                                            const Vec3f &target,
                                            const Vec3f &camera_pos,
                                            const Vec3f &up,
                                            float fov,
                                            float near_clip,
                                            float far_clip) : Camera(image_width, image_height, target, camera_pos, up),
                                                              fov_(fov),
                                                              near_clip_(near_clip),
                                                              far_clip_(far_clip) {
  image_aspect_ = static_cast<float>(image_width) / static_cast<float>(image_height);
  float cot = 1.0f / std::tan(DegToRad(0.5f * fov_));
  screen2raster_ = Scale(-0.5f, -0.5f * image_aspect_, 1.0f)
      * Translate(-1.0f, -1.0f / image_aspect_, 0.0f);
  raster2screen_ = screen2raster_.inverse();
  camera2screen_ = Perspective(cot, cot, near_clip_, far_clip_);
  camera2raster_ = screen2raster_ * camera2screen_;
  raster2camera_ = Inverse(camera2raster_);
  world2raster_ = camera2raster_ * world2camera_;
  raster2_world_ = Inverse(world2raster_);
}
beart::Ray beart::PerspectiveCamera::GenerateRay(const float &x,
                                                 const float &y,
                                                 const beart::PixelSample &pixel_sample) const noexcept {
  Vec3f p{(x + pixel_sample.image_u_) * inv_image_width_, (y + pixel_sample.image_v_) * inv_image_height_, 0.0f};

  Vec3f dir = Normalize(TransformPoint(raster2camera_, p));
  float inv_z = 1.0f / dir.z();
  dir = TransformVector(camera2world_, dir);
  Vec3f ori = TransformPoint(camera2world_, Vec3f{0.f, 0.f, 0.f});
  Ray ray{ori, dir, 1, near_clip_ * inv_z, far_clip_ * inv_z, true};
  return ray;
}
