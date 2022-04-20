#include "shape.h"
#include "sphere.h"
#include "perspective_camera.h"
#include "samples.h"
#include "scene.h"
#include "primitive.h"
#include "point_light.h"
#include "direct.h"
#include "phong.h"
#include "normal.h"
#include "directional_light.h"

#include <iostream>
int main() {
  auto phong = std::make_unique<beart::Phong>(beart::Spectrum{0.5, 0.5, 0.5}, beart::Spectrum{0.2, 0.2, 0.2}, 30.0);
//  auto phong = std::make_unique<beart::NormalMaterial>();

  std::unique_ptr<beart::Shape> sphere1 = std::make_unique<beart::Sphere>(beart::Vec3f{-0.2, 0.2, -3.f}, 0.1);
  auto sphere1_prim = std::make_unique<beart::Primitive>(sphere1.get(), phong.get());
  std::unique_ptr<beart::Shape> sphere2 = std::make_unique<beart::Sphere>(beart::Vec3f{-0.1f, -0.1, -3.f}, 0.2f);
  auto sphere2_prim = std::make_unique<beart::Primitive>(sphere2.get(), phong.get());
//  std::unique_ptr<beart::Shape> sphere3 = std::make_unique<beart::Sphere>(beart::Vec3f{-0.2f, 0.2f, -3.f}, 0.1f);
//  auto sphere3_prim = std::make_unique<beart::Primitive>(sphere3.get(), phong.get());
//  std::unique_ptr<beart::Shape> sphere4 = std::make_unique<beart::Sphere>(beart::Vec3f{-0.1f, -0.1f, -3.f}, 0.2f);
//  auto sphere4_prim = std::make_unique<beart::Primitive>(sphere4.get(), phong.get());
//  auto camera = std::make_unique<const beart::PerspectiveCamera>(700, 900);
  auto camera = std::make_unique<const beart::PerspectiveCamera>(700,
                                                                 900,
                                                                 beart::Vec3f{0, 0, -1},
                                                                 beart::Vec3f{0, 0, 0},
                                                                 beart::Vec3f{0, 1, 0},
                                                                 1.57);
  auto point_light =
      std::make_unique<beart::PointLight>(beart::Vec3f{0.2f, 0.1f, -2}, beart::Spectrum{1, 1, 1});
  auto distant_light =
      std::make_unique<beart::DirectionalLight>(beart::Vec3f{0.2f, 0.1f, -2}, beart::Spectrum{1, 1, 1});
  auto integrator = beart::Direct{};

  beart::Scene scene;
  scene.AddPrimitive(sphere1_prim.get());
  scene.AddPrimitive(sphere2_prim.get());
  scene.AddLight(point_light.get());
//  scene.AddLight(distant_light.get());
  scene.set_camera(camera.get());
  scene.Prepare();
  FILE *fp = fopen("sphere_normal.ppm", "wb");
  (void) fprintf(fp, "P3\n%d %d\n255\n", scene.camera()->image_width(), scene.camera()->image_height());
  for (unsigned j = scene.camera()->image_height(); j > 0; --j) {
    for (unsigned int i = 0; i < scene.camera()->image_width(); ++i) {
      // TODO: generate sample by i, j
      auto ps = beart::PixelSample{0.f, 0.f};
      beart::Ray r = scene.camera()->GenerateRay(i, j, ps);
      auto nor = integrator.Li(r, scene, ps);
      if (nor.maxCoeff() >= 1.0) {
        nor = nor / nor.maxCoeff();
      }
      (void) fprintf(fp,
                     "%d %d %d\n",
                     static_cast<int>(255.999 * nor.x()),
                     static_cast<int>(255.999 * nor.y()),
                     static_cast<int>(255.999 * nor.z()));
    }
  }
  return 0;
}
