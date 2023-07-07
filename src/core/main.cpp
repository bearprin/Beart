#include "shape.h"
#include "sphere.h"
#include "perspective_camera.h"

#include <iostream>
int main() {
  auto camera = std::make_unique<beart::PerspectiveCamera>(800,
                                                           600,
                                                           beart::Vec3f{0, 0, 0},
                                                           beart::Vec3f{0, 0.0, -1},
                                                           beart::Vec3f{0, 1, 0},
                                                           60.0);
  std::unique_ptr<beart::Shape> sphere1 = std::make_unique<beart::Sphere>(beart::Vec3f{0, 0, 1.f}, 0.1);
  for (unsigned j = 0; j < camera->image_height(); ++j) {
    for (unsigned int i = 0; i < camera->image_width(); ++i) {
      auto ps = beart::PixelSample{0.f, 0.f};
      beart::Ray r = camera->GenerateRay(i, j, ps);
      auto hit = sphere1->Intersect(r, nullptr);
      if (hit) {
        std::cout << i << " " << j << " " << std::endl;
        camera->image()->set_color(i, j, {1., 0, 0,});
      }
    }
  }
  std::cout << camera->image()->Save("test.png");
  return 0;
}
