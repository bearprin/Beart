#include "shape.h"
#include "sphere.h"
#include "perspective_camera.h"
#include "scene.h"
#include "normal.h"
#include "ao.h"
#include "direct.h"
#include "diffuse.h"
#include "point_light.h"
#include "directional_light.h"
#include "phong.h"
#include "random_sampler.h"
#include "triangle_mesh.h"
#include <OpenImageDenoise/oidn.hpp>
int main() {
  auto camera = std::make_unique<beart::PerspectiveCamera>(768,
                                                           768,
                                                           beart::Vec3f{0, 0, 0},
                                                           beart::Vec3f{0, 0.5, 1},
                                                           beart::Vec3f{0, 1, 0},
                                                           16.0);
  std::unique_ptr<beart::Shape> sphere1 = std::make_unique<beart::Sphere>(beart::Vec3f{0., 0.0, 0}, 0.05);
  std::unique_ptr<beart::Shape> plane = std::make_unique<beart::TriangleMesh>("../../../asset/plane.obj",
                                                                              beart::Translate({0, -0.05, 0})
                                                                                  * beart::Scale({1, 1, 1}));

//  auto integrator = std::make_unique<beart::DirectIntegrator>();
  auto integrator = std::make_unique<beart::AOIntegrator>();
//  auto integrator = std::make_unique<beart::NormalIntegrator>();
  auto sampler = std::make_unique<beart::RandomSampler>();
  auto diffust_material = std::make_shared<beart::Diffuse>(beart::Spectrum{0.5, 0.5, 0.5});
  auto phone_material = std::make_shared<beart::Phong>();
  auto point_light = std::make_unique<beart::PointLight>(beart::Point3f{0.2, 0.2, 0}, beart::Spectrum{0.1});
  auto dir_light = std::make_unique<beart::DirectionalLight>(beart::Vec3f{-1, -1, 0}, beart::Spectrum{2});
  beart::Scene scene;
  beart::Primitive a{sphere1.get(), phone_material};
  beart::Primitive b{plane.get(), phone_material};
  scene.AddPrimitive(&a);
  scene.AddPrimitive(&b);
//  scene.AddPrimitive(&c);
  scene.AddLight(dir_light.get());
//  scene.AddLight(point_light.get());
  scene.Prepare();

  uint sample_count = 1;
  for (unsigned j = 0; j < camera->image_height(); ++j) {
    for (unsigned int i = 0; i < camera->image_width(); ++i) {
      auto L = beart::Spectrum{0.};
      for (unsigned int k = 0; k < sample_count; ++k) {
        auto ps = beart::PixelSample{sampler->Next1D(), sampler->Next1D()};
        beart::Ray r = camera->GenerateRay(i, j, ps);
        L += integrator->Li(r, scene, *sampler);
      }
      L = L / sample_count;
      camera->image()->set_color(i, j, L);
    }
  }
  std::cout << camera->image()->Save("AO.png");


//  // Create an Open Image Denoise device
//  oidn::DeviceRef device = oidn::newDevice(); // CPU or GPU if available
//  device.commit();
//
//// Create buffers for input/output images accessible by both host (CPU) and device (CPU/GPU)
//  oidn::BufferRef colorBuf = device.newBuffer(camera->image_width() * camera->image_height() * 3 * sizeof(float));
//  oidn::BufferRef outputBuf = device.newBuffer(camera->image_width() * camera->image_height() * 3 * sizeof(float));
//  oidn::FilterRef filter = device.newFilter("RT");
//  filter.setImage("color", colorBuf, oidn::Format::Float3, camera->image_width(), camera->image_height());
//  filter.setImage("output", outputBuf, oidn::Format::Float3, camera->image_width(), camera->image_height());
//
//  filter.set("srgb", false);
//  filter.commit();
//
//  const char *errorMessage;
//  if (device.getError(errorMessage) != oidn::Error::None) std::cout << "Error: " << errorMessage << std::endl;
//  float *colorPtr = (float *) colorBuf.getData();
//  for (unsigned j = 0; j < camera->image_height(); ++j) {
//    for (unsigned int i = 0; i < camera->image_width(); ++i) {
//      auto color = camera->image()->color(i, j);
//      colorPtr[(j * camera->image_width() + i) * 3 + 0] = color.x();
//      colorPtr[(j * camera->image_width() + i) * 3 + 1] = color.y();
//      colorPtr[(j * camera->image_width() + i) * 3 + 2] = color.z();
//    }
//  }
//
//  filter.execute();
//
//  auto new_colorPtr = (float*)outputBuf.getData();
//  for (unsigned j = 0; j < camera->image_height(); ++j) {
//    for (unsigned int i = 0; i < camera->image_width(); ++i) {
//      auto color = camera->image()->color(i, j);
//      color.x() = new_colorPtr[(j * camera->image_width() + i) * 3 + 0];
//      color.y() = new_colorPtr[(j * camera->image_width() + i) * 3 + 1];
//      color.z() = new_colorPtr[(j * camera->image_width() + i) * 3 + 2];
//      camera->image()->set_color(i, j, color);
//    }
//  }

  return 0;
}
