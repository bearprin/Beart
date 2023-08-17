//
// Created by Bear on 2023/7/7.
//

#pragma once

#include "json_serializable.h"
#include "vec.h"
#include "transform.h"
#include "film.h"
#include "ray.h"
#include "spectrum.h"
#include "samples.h"

namespace beart {
class SurfaceInterection;
class Sampler;
class Camera {
 public:
  Camera(unsigned int image_width,
         unsigned int image_height,
         const Vec3f &target = Vec3f{0.f, 0.f, 0.0f},
         const Vec3f &camera_pos = Vec3f{0.f, 0.f, 0.0f},
         const Vec3f &up = Vec3f{0.f, 1.f, 0.0f})
      : image_width_(image_width),
        image_height_(image_height),
        inv_image_width_(1.f / static_cast<float>(image_width)),
        inv_image_height_(1.f / static_cast<float>(image_height)), target_(target), camera_pos_(camera_pos), up_(up) {
    image_ptr_ = std::make_unique<Film<RGBSpectrum>>(image_width_, image_height_);
    normal_ptr_ = std::make_unique<Film<RGBSpectrum>>(image_width_, image_height_);

    camera_to_world_ = LookAt(camera_pos, target, up);
    world_to_camera_ = Inverse(camera_to_world_);
  }
  Camera(unsigned int image_width, unsigned int image_height, Transform camera_2_world)
      : image_width_(image_width), image_height_(image_height), camera_to_world_(std::move(camera_2_world)) {
    image_ptr_ = std::make_unique<Film<RGBSpectrum>>(image_width_, image_height_);
    normal_ptr_ = std::make_unique<Film<RGBSpectrum>>(image_width_, image_height_);

    world_to_camera_ = Transform{Inverse(camera_to_world_)};
  }
  Camera(const json &j) {

  }

  virtual ~Camera() = default;;
  unsigned int image_width() const {
    return image_width_;
  }
  unsigned int image_height() const {
    return image_height_;
  }
  std::pair<unsigned int, unsigned int> ImageResolution() const {
    return {image_width_, image_height_};
  }
  const Transform &camera_to_world() const {
    return camera_to_world_;
  }
  const Transform &world_to_camera() const {
    return world_to_camera_;
  }
  Vec3f forward() const {
    return Normalize(target_ - camera_pos_);
  }
  Film<RGBSpectrum> *image() const {
    return image_ptr_.get();
  }
  Film<RGBSpectrum> *normal() const {
    return normal_ptr_.get();
  }
  virtual Ray GenerateRay(const float &x, const float &y, const PixelSample &pixel_sample) const noexcept = 0;
  virtual Vec2i GetScreenCoord(const SurfaceInterection &inter,
                               beart::Visibility &visibility,
                               const Sampler &sampler,
                               float &cos_camera,
                               float *pdf_solid,
                               float *pdf_area,
                               beart::Spectrum *we,
                               beart::Point3f *eye_p) const noexcept = 0;
 protected:
  unsigned int image_width_;
  unsigned int image_height_;
  float inv_image_width_;
  float inv_image_height_;
  Transform camera_to_world_;
  Transform world_to_camera_;
  Vec3f target_;
  Vec3f camera_pos_;
  Vec3f up_;

  std::unique_ptr<Film<RGBSpectrum>> image_ptr_;  // image buffer
  std::unique_ptr<Film<RGBSpectrum>> normal_ptr_;  // normal buffer
};
}