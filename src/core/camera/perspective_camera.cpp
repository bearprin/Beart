//
// Created by Bear on 2023/07/6.
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
  // Translate and scale to shift the clip coordinates into the range from zero to one, and take the aspect ratio into account.
  screen_to_raster_ = Scale({-0.5f, -0.5f * image_aspect_, 1.0f}) * Translate({-1.0f, -1.0f / image_aspect_, 0.0f});
  raster_to_screen_ = Inverse(screen_to_raster_);
  // Create transform from camera space to [-1,1]x[-1,1]x[0,1] clip coordinates (not taking account of the aspect ratio yet)
  camera_to_screen_ = Perspective(fov_,
                                  near_clip_,
                                  far_clip_);
  camera_to_raster_ = screen_to_raster_ * camera_to_screen_;
  raster_to_camera_ = Inverse(camera_to_raster_);
  world_to_raster_ = camera_to_raster_ * world_to_camera_;
  raster_to_world_ = Inverse(world_to_raster_);
}
beart::Ray beart::PerspectiveCamera::GenerateRay(const float &x,
                                                 const float &y,
                                                 const beart::PixelSample &pixel_sample) const noexcept {
  // scale to the [0, 1] range
  Vec3f p{(x + pixel_sample.image_u_) * inv_image_width_, (y + pixel_sample.image_v_) * inv_image_height_, 0.0f};
  Vec3f dir = Normalize(raster_to_camera_.TransformPoint(p));  // transform to camera space
  float inv_z = 1.0f / dir.z();  // scale to the range of ray [t_min, t_max]
  dir = camera_to_world_.TransformVector(dir);
  Vec3f ori = camera_to_world_.TransformPoint(Vec3f{0.f, 0.f, 0.f}); // transform to world space (camera origin)
  Ray ray{ori, dir, 0, true, near_clip_ * inv_z, far_clip_ * inv_z};
  return ray;
}
