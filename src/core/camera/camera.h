//
// Created by Bear on 2021/12/3.
//

#pragma once

#include "vec.h"
#include "transform.h"
#include "film.h"
#include "ray.h"
#include "spectrum.h"
#include "samples.h"

namespace beart {
class Camera {
 public:
  Camera(unsigned int image_width,
         unsigned int image_height,
         const Vec3f &camera_pos = Vec3f{0.f, 0.f, 0.0f},
         const Vec3f &up = Vec3f{0.f, 1.f, 0.0f},
         const Vec3f &dir = Vec3f{0.f, 0.f, -1.0f}) : image_width_(image_width), image_height_(image_height) {
    image_ptr_ = std::make_unique<Film<RGBSpectrum>>(image_width_, image_height_);
    normal_ptr_ = std::make_unique<Film<RGBSpectrum>>(image_width_, image_height_);

    camera2world_ = LookAt(camera_pos, up, dir);
    world2camera_ = camera2world_.inverse();
  }
  Camera(unsigned int image_width, unsigned int image_height, Transform camera_2_world)
      : image_width_(image_width), image_height_(image_height), camera2world_(std::move(camera_2_world)) {
    image_ptr_ = std::make_unique<Film<RGBSpectrum>>(image_width_, image_height_);
    normal_ptr_ = std::make_unique<Film<RGBSpectrum>>(image_width_, image_height_);

    world2camera_ = camera2world_.inverse();
  }
  [[nodiscard]] unsigned int image_width() const {
    return image_width_;
  }
  [[nodiscard]] unsigned int image_height() const {
    return image_height_;
  }
  [[nodiscard]] std::tuple<unsigned int, unsigned int> imageResolution() const {
    return {image_width_, image_height_};
  }
  virtual Ray GenerateRay(const float &x, const float &y, const PixelSample &pixel_sample) const noexcept = 0;
 protected:
  unsigned int image_width_;
  unsigned int image_height_;
  Transform camera2world_;
  Transform world2camera_;

  std::unique_ptr<Film<RGBSpectrum>> image_ptr_;
  std::unique_ptr<Film<RGBSpectrum>> normal_ptr_;
};
}