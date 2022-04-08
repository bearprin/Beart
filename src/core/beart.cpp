#include "primitive.h"
#include "sphere.h"
#include "perspective_camera.h"
#include "samples.h"
#include "scene.h"

#include <iostream>
int main() {
  auto sphere1 = std::make_unique<const beart::Sphere>(beart::Vec3f{0.f, 0.f, -8.f}, 0.5f);
  auto sphere2 = std::make_unique<const beart::Sphere>(beart::Vec3f{-0.1f, 0.2f, -3.f}, 0.1f);
  auto sphere3 = std::make_unique<const beart::Sphere>(beart::Vec3f{0.1f, 0.2f, -3.f}, 0.1f);
  auto sphere4 = std::make_unique<const beart::Sphere>(beart::Vec3f{-0.1f, -0.1f, -3.f}, 0.1f);
  auto camera = std::make_unique<const beart::PerspectiveCamera>(400, 300);
  beart::Scene scene;
  scene.AddPrimitive(sphere1.get());
  scene.AddPrimitive(sphere2.get());
  scene.AddPrimitive(sphere3.get());
  scene.AddPrimitive(sphere4.get());
  scene.set_camera(camera.get());
  scene.Prepare();
  FILE *fp = fopen("sphere_normal.ppm", "wb");
  (void) fprintf(fp, "P3\n%d %d\n255\n", 400, 300);
  for (unsigned j = scene.camera()->image_height(); j > 0; --j) {
    for (unsigned int i = 0; i < scene.camera()->image_width(); ++i) {
      // TODO: generate sample by i, j
      beart::Ray r = scene.camera()->GenerateRay(i, j, beart::PixelSample{0.f, 0.f});
      auto info = std::make_unique<beart::IntersectionInfo>();
      if (scene.IntersectInfo(r, info.get())) {
//      if (sphere1->IntersectInfo(r, info.get())) {
//        beart::Vec3f nor = (info->Ns + beart::Vec3f(1, 1, 1)) * 0.5;
//        auto nor = beart::Vec3f{1.f, 0.f, 0.f};
        beart::Vec3f nor = (info->Ns + beart::Vec3f(1, 1, 1)) * 0.5;
        (void) fprintf(fp,
                       "%d %d %d\n",
                       static_cast<int>(255.999 * nor.x()),
                       static_cast<int>(255.999 * nor.y()),
                       static_cast<int>(255.999 * nor.z()));
      } else {
        (void) fprintf(fp, "0 0 0\n");
      }
    }
  }
  return 0;
}
