//
// Created by Bear on 2021/12/6.
//

#include "PerspectiveCamera.h"

beart::PerspectiveCamera::PerspectiveCamera(unsigned int image_width,
                                            unsigned int image_height,
                                            const Vec3f &camera_pos,
                                            const Vec3f &up,
                                            const Vec3f &dir,
                                            float near_clip,
                                            float far_clip,
                                            float fov) : Camera(image_width, image_height, camera_pos, up, dir),
                                                         near_clip_(near_clip),
                                                         far_clip_(far_clip),
                                                         fov_(fov) {
  image_aspect_ = static_cast<float>(image_width) / static_cast<float>(image_height);
  float scale_y = 1.0f / std::tan(0.5f * fov_);
  float scale_x = scale_y / image_aspect_;

  raster2screen_ = Translate(-1.0f, 1.0f, 0.0f) * Scale(2.0f, -2.0f, 1.0f)
      * Scale(1.0f / static_cast<float>(image_width_), 1.0f / static_cast<float>(image_height_), 1.0f);
  screen2raster_ = raster2screen_.inverse();

  camera2screen_ = Perspective(scale_x, scale_y, near_clip_, far_clip_);
  camera2raster_ = screen2raster_ * camera2screen_;
  raster2camera_ = camera2raster_.inverse();

  world2raster_ = camera2raster_ * world2camera_;
  raster2_world_ = world2raster_.inverse();
}
beart::Ray beart::PerspectiveCamera::GenerateRay(const float &x,
                                                 const float &y,
                                                 const beart::PixelSample &pixel_sample) const noexcept {
  Vec3f p{x + pixel_sample.image_u, y + pixel_sample.image_v, 0.0f};

  // TODO: add the depth of field
  Vec3f dir = TransformPoint(raster2_world_, p);
  Vec3f ori = TransformPoint(raster2_world_, Vec3f{0.f, 0.f, 0.f});
  Ray ray{ori, dir, 1, true};
  return ray;
}
