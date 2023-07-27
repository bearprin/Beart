#include "shape.h"
#include "sphere.h"
#include "perspective_camera.h"
#include "scene.h"
#include "ao.h"
#include "direct.h"
#include "diffuse.h"
#include "point_light.h"
#include "directional_light.h"
#include "area_light.h"
#include "phong.h"
#include "random_sampler.h"
#include "triangle_mesh.h"
#include "path_tracing.h"

#include <nanothread/nanothread.h>
#include <OpenImageDenoise/oidn.hpp>
namespace dr = drjit;
int main(int argc, char **argv) {
//  auto camera = std::make_unique<beart::PerspectiveCamera>(768,
//                                                           768,
//                                                           beart::Vec3f{0, 0, 0},
//                                                           beart::Vec3f{0, 0.5, -1},
//                                                           beart::Vec3f{0, 1, 0},
//                                                           16.0);
//  std::unique_ptr<beart::Shape> sphere1 = std::make_unique<beart::Sphere>(beart::Vec3f{0., 0.0, 0}, 0.05);
////  std::unique_ptr<beart::Shape> sphere2 = std::make_unique<beart::Sphere>(beart::Vec3f{1., -1.5, 1.75}, 0.1);
//  std::unique_ptr<beart::Shape> plane = std::make_unique<beart::TriangleMesh>(argv[1],
//                                                                              beart::Translate({0, -0.05, 0})
//                                                                                  * beart::Scale({0.2, 0.1, 0.2}));
//  auto integrator = std::make_unique<beart::DirectIntegrator>();
//  auto sampler = std::make_unique<beart::RandomSampler>();
//  auto diffuse_material = std::make_shared<beart::Diffuse>(beart::Spectrum{0.5, 0.5, 0.5});
//  auto phone_material = std::make_shared<beart::Phong>();
//  auto point_light = std::make_unique<beart::PointLight>(beart::Point3f{0.2, 0.2, 0}, beart::Spectrum{.1});
//  auto dir_light = std::make_unique<beart::DirectionalLight>(beart::Vec3f{-1, -1, 0}, beart::Spectrum{2});
//  auto area_light =
//      std::make_unique<beart::AreaLight>(std::make_unique<beart::Sphere>(beart::Point3f{1, -1.5, 1.75}, 0.1),
//                                         beart::Spectrum{300});

//  auto camera = std::make_unique<beart::PerspectiveCamera>(768,
//                                                           768,
//                                                           beart::Vec3f{0, 0, 0},
//                                                           beart::Vec3f{4, 4, 0},
//                                                           beart::Vec3f{0, 0, 1},
//                                                           40.0);
//  std::unique_ptr<beart::Shape> sphere1 = std::make_unique<beart::Sphere>(beart::Vec3f{0., 0.0, 0}, 1);
////  std::unique_ptr<beart::Shape> sphere2 = std::make_unique<beart::Sphere>(beart::Vec3f{1., -1.5, 1.75}, 0.1);
//  std::unique_ptr<beart::Shape> plane = std::make_unique<beart::TriangleMesh>("../../../asset/plane.obj",
//                                                                              beart::Translate({0, 0, -1})
//                                                                                  * beart::Rotate({1, 0, 0}, 90)
//                                                                                  * beart::Scale({100, 1, 100}));
//
//  auto integrator = std::make_unique<beart::DirectIntegrator>();
////  auto integrator = std::make_unique<beart::AOIntegrator>();
////  auto integrator = std::make_unique<beart::NormalIntegrator>();
//  auto sampler = std::make_unique<beart::RandomSampler>();
//  auto diffuse_material = std::make_shared<beart::Diffuse>(beart::Spectrum{0.5, 0.5, 0.5});
//  auto phone_material = std::make_shared<beart::Phong>();
//  auto point_light = std::make_unique<beart::PointLight>(beart::Point3f{1, -1.5, 1.75}, beart::Spectrum{1});
//  auto dir_light = std::make_unique<beart::DirectionalLight>(beart::Vec3f{-1, -1, 0}, beart::Spectrum{2});
//  auto area_light =
//      std::make_unique<beart::AreaLight>(std::make_unique<beart::Sphere>(beart::Point3f{1, -1.5, 1.75}, 0.1),
//                                         beart::Spectrum{300});
//  beart::Scene scene;
//  beart::Primitive a{sphere1.get(), phone_material};
//  beart::Primitive b{plane.get(), diffuse_material};
//  beart::Primitive c{area_light->shape(), area_light.get()};
  // hw3 t1 t2 scene
//  auto camera = std::make_unique<beart::PerspectiveCamera>(768,
//                                                           576,
//                                                           beart::Vec3f{0, 0, 0},
//                                                           beart::Vec3f{4, 4, 0},
//                                                           beart::Vec3f{0, 0, 1},
//                                                           40.0);
//  std::unique_ptr<beart::Shape> sphere1 = std::make_unique<beart::Sphere>(beart::Vec3f{0., 0.0, 0}, 1);
//  std::unique_ptr<beart::Shape> plane = std::make_unique<beart::TriangleMesh>("../../../asset/plane.obj",
//                                                                              beart::Translate({0, 0, -1})
//                                                                                  * beart::Rotate({1, 0, 0}, 90)
//                                                                                  * beart::Scale({100, 1, 100}));
//  std::unique_ptr<beart::Shape> bunny = std::make_unique<beart::TriangleMesh>("../../../asset/bunny.obj",
//                                                                              beart::Translate({-0.3, 0, -0.8}) *
//                                                                                  beart::Rotate({0, 0, 1}, 120)
//                                                                                  * beart::Rotate({1, 0, 0}, 90)
//                                                                                  * beart::Scale({0.7, 0.7, 0.7}));
//  auto area_light =
//      std::make_unique<beart::AreaLight>(std::make_unique<beart::Sphere>(beart::Point3f{1, 2, 2}, 0.2),
//                                         beart::Spectrum{300});
//
//  auto diffuse_material = std::make_shared<beart::Diffuse>(beart::Spectrum{0.5, 0.5, 0.5});
//  auto phong_material = std::make_shared<beart::Phong>(beart::Spectrum{0.2, 0.2, 0.8}, beart::Spectrum{1.0}, 40);
//
//  auto sampler = std::make_unique<beart::RandomSampler>();
//
//  beart::Scene scene;
//
////  beart::Primitive a{sphere1.get(), phong_material};
//  beart::Primitive a{bunny.get(), phong_material};
//  beart::Primitive b{plane.get(), diffuse_material};
//  beart::Primitive c{area_light->shape(), area_light.get()};

  // hw3 t3
//  auto camera = std::make_unique<beart::PerspectiveCamera>(768,
//                                                           512,
//                                                           beart::Vec3f{0, -2, 2.5},
//                                                           beart::Vec3f{0, 2, 15},
//                                                           beart::Vec3f{0, 1, 0},
//                                                           42.0);
//  auto integrator = std::make_unique<beart::DirectIntegrator>();
//  auto integrator = std::make_unique<beart::PathTracing>();
//  auto sampler = std::make_unique<beart::RandomSampler>();

//  auto env = std::make_unique<beart::AreaLight>(std::make_unique<beart::Sphere>(beart::Point3f{10, 10, 4}, 0.5),
//                                                beart::Spectrum{800, 800, 800});
//  auto red =
//      std::make_unique<beart::AreaLight>(std::make_unique<beart::Sphere>(beart::Point3f{-3.75, 0, 0}, 0.03333),
//                                         beart::Spectrum{901.803, 0, 0});
//  auto green =
//      std::make_unique<beart::AreaLight>(std::make_unique<beart::Sphere>(beart::Point3f{-1.25, 0, 0}, 0.1),
//                                         beart::Spectrum{0, 100, 0});
//  auto blue = std::make_unique<beart::AreaLight>(std::make_unique<beart::Sphere>(beart::Point3f{1.25, 0, 0}, 0.3),
//                                                 beart::Spectrum{0, 0, 11.1111});
//  auto white = std::make_unique<beart::AreaLight>(std::make_unique<beart::Sphere>(beart::Point3f{3.75, 0, 0}, 0.9),
//                                                  beart::Spectrum{1.23457, 1.23457, 0});
//
//  std::unique_ptr<beart::Shape> plate1 = std::make_unique<beart::TriangleMesh>("../../../asset/plate1.obj");
//  std::unique_ptr<beart::Shape> plate2 = std::make_unique<beart::TriangleMesh>("../../../asset/plate2.obj");
//  std::unique_ptr<beart::Shape> plate3 = std::make_unique<beart::TriangleMesh>("../../../asset/plate3.obj");
//  std::unique_ptr<beart::Shape> plate4 = std::make_unique<beart::TriangleMesh>("../../../asset/plate4.obj");
//  std::unique_ptr<beart::Shape> floor = std::make_unique<beart::TriangleMesh>("../../../asset/floor.obj");
//
//  auto phong_material_1 =
//      std::make_shared<beart::Phong>(beart::Spectrum{0.07, 0.09, 0.13}, beart::Spectrum{0.93, 0.91, 0.87}, 10000);
//  auto phong_material_2 =
//      std::make_shared<beart::Phong>(beart::Spectrum{0.07, 0.09, 0.13}, beart::Spectrum{0.93, 0.91, 0.87}, 1000);
//  auto phong_material_3 =
//      std::make_shared<beart::Phong>(beart::Spectrum{0.07, 0.09, 0.13}, beart::Spectrum{0.93, 0.91, 0.87}, 100);
//  auto phong_material_4 =
//      std::make_shared<beart::Phong>(beart::Spectrum{0.07, 0.09, 0.13}, beart::Spectrum{0.93, 0.91, 0.87}, 80);
//  auto diffuse_material = std::make_shared<beart::Diffuse>(beart::Spectrum{0.4, 0.4, 0.4});
//  auto diffuse_material_env = std::make_shared<beart::Diffuse>(beart::Spectrum{0.});
//
//  beart::Primitive a{plate1.get(), phong_material_1};
//  beart::Primitive b{plate2.get(), phong_material_2};
//  beart::Primitive c{plate3.get(), phong_material_3};
//  beart::Primitive d{plate4.get(), phong_material_4};
//  beart::Primitive e{floor.get(), diffuse_material};

//  beart::Primitive r{red->shape(), red.get()};
//  beart::Primitive g{green->shape(), green.get()};
//  beart::Primitive bl{blue->shape(), blue.get()};
//  beart::Primitive w{white->shape(), white.get()};
//  beart::Primitive env_p{env->shape(), env.get(), diffuse_material_env};

//  beart::Scene scene;
//  scene.AddPrimitive(&a);
//  scene.AddPrimitive(&b);
//  scene.AddPrimitive(&c);
//  scene.AddPrimitive(&d);
//  scene.AddPrimitive(&e);
//  scene.AddPrimitive(&r);
//  scene.AddPrimitive(&g);
//  scene.AddPrimitive(&bl);
//  scene.AddPrimitive(&w);

//  scene.AddLight(red.get());
//  scene.AddLight(green.get());
//  scene.AddLight(blue.get());
//  scene.AddLight(white.get());
//  scene.Prepare();

  auto camera = std::make_unique<beart::PerspectiveCamera>(768,
                                                           768,
                                                           beart::Vec3f{0, 0, 1},
                                                           beart::Vec3f{0, -4, 1},
                                                           beart::Vec3f{0, 0, 1},
                                                           37.0);
  auto integrator = std::make_unique<beart::PathTracing>();
//  auto integrator = std::make_unique<beart::DirectIntegrator>();
  auto sampler = std::make_unique<beart::RandomSampler>();

  std::unique_ptr<beart::Shape> wall = std::make_unique<beart::TriangleMesh>("../../../asset/cornellbox_walls.obj");
  std::unique_ptr<beart::Shape> lwall = std::make_unique<beart::TriangleMesh>("../../../asset/cornellbox_lwall.obj");
  std::unique_ptr<beart::Shape> rwall = std::make_unique<beart::TriangleMesh>("../../../asset/cornellbox_rwall.obj");
  std::unique_ptr<beart::Shape> bigbox = std::make_unique<beart::TriangleMesh>("../../../asset/cornellbox_bigbox.obj");
  std::unique_ptr<beart::Shape>
      smallbox = std::make_unique<beart::TriangleMesh>("../../../asset/cornellbox_smallbox.obj");

  auto area_light =
      std::make_unique<beart::AreaLight>(std::make_unique<beart::Sphere>(beart::Point3f{0, 0, 1.9}, 0.05),
                                         beart::Spectrum{300});

  auto diffuse_material = std::make_shared<beart::Diffuse>(beart::Spectrum{0.725, 0.71, 0.68});
  auto diffuse_material_l = std::make_shared<beart::Diffuse>(beart::Spectrum{0.05, 0.21, 0.63});
  auto diffuse_material_r = std::make_shared<beart::Diffuse>(beart::Spectrum{0.63, 0.65, 0.05});

  beart::Primitive a{wall.get(), diffuse_material};
  beart::Primitive b{lwall.get(), diffuse_material_l};
  beart::Primitive c{rwall.get(), diffuse_material_r};
  beart::Primitive d{bigbox.get(), diffuse_material};
  beart::Primitive e{smallbox.get(), diffuse_material};

  beart::Primitive light{area_light->shape(), area_light.get()};

  beart::Scene scene;
  scene.AddPrimitive(&a);
  scene.AddPrimitive(&b);
  scene.AddPrimitive(&c);
  scene.AddPrimitive(&d);
  scene.AddPrimitive(&e);
  scene.AddPrimitive(&light);

  scene.AddLight(area_light.get());

  scene.Prepare();
  uint sample_count = 16;
  for (unsigned j = 0; j < camera->image_height(); ++j) {
    dr::parallel_for(
        dr::blocked_range<uint32_t>(/* begin = */ 0, /* end = */ camera->image_width(), /* block_size = */ 32),
        [&](dr::blocked_range<uint32_t> range) {
          for (uint32_t i = range.begin(); i != range.end(); ++i) {
//    for (uint32_t i = 0u; i != camera->image_width(); ++i) {
            auto L = beart::Spectrum{0.};
            auto normal = beart::Spectrum{0.};
            auto albendo = beart::Spectrum{0.};
            for (unsigned int k = 0; k < sample_count; ++k) {
              auto ps = beart::PixelSample{sampler->Next1D(), sampler->Next1D()};
              beart::Ray r = camera->GenerateRay(i, j, ps);
              L += integrator->Li(r, scene, *sampler, &normal, &albendo);
            }
            L = L / sample_count;
            normal = normal / sample_count;
            albendo = albendo / sample_count;
            camera->image()->set_color(i, j, L);
            camera->normal()->set_color(i, j, normal);
          }
        });
  }
  camera->image()->Save("hw4_pt_nodenoise.exr");
  camera->normal()->Save("hw4_pt_normal.exr");


  // Create an Open Image Denoise device
  oidn::DeviceRef device = oidn::newDevice(); // CPU or GPU if available
  device.commit();

// Create buffers for input/output images accessible by both host (CPU) and device (CPU/GPU)
  oidn::BufferRef colorBuf = device.newBuffer(camera->image_width() * camera->image_height() * 3 * sizeof(float));
  oidn::BufferRef normalBuf = device.newBuffer(camera->image_width() * camera->image_height() * 3 * sizeof(float));
  oidn::FilterRef filter = device.newFilter("RT");
  filter.setImage("color", colorBuf, oidn::Format::Float3, camera->image_width(), camera->image_height());
  filter.setImage("output", colorBuf, oidn::Format::Float3, camera->image_width(), camera->image_height());
//  filter.setImage("normal", normalBuf, oidn::Format::Float3, camera->image_width(), camera->image_height());
//  filter.setImage("albedo", normalBuf, oidn::Format::Float3, camera->image_width(), camera->image_height());
  filter.set("hdr", true);
  filter.commit();

  const char *errorMessage;
  if (device.getError(errorMessage) != oidn::Error::None) std::cout << "Error: " << errorMessage << std::endl;
  float *colorPtr = (float *) colorBuf.getData();
  float *normalPtr = (float *) normalBuf.getData();
  for (unsigned j = 0; j < camera->image_height(); ++j) {
    for (unsigned int i = 0; i < camera->image_width(); ++i) {
      auto color = camera->image()->color(i, j);
      colorPtr[(j * camera->image_width() + i) * 3 + 0] = color.x();
      colorPtr[(j * camera->image_width() + i) * 3 + 1] = color.y();
      colorPtr[(j * camera->image_width() + i) * 3 + 2] = color.z();

      auto normal = camera->normal()->color(i, j);
      // map [0, 1] to [-1, 1]
//      normal = normal * 2 - 1;
//      std::cout << normal << std::endl;
      normalPtr[(j * camera->image_width() + i) * 3 + 0] = normal.x();
      normalPtr[(j * camera->image_width() + i) * 3 + 1] = normal.y();
      normalPtr[(j * camera->image_width() + i) * 3 + 2] = normal.z();

    }
  }

  filter.execute();

  auto new_colorPtr = (float *) colorBuf.getData();
  for (unsigned j = 0; j < camera->image_height(); ++j) {
    for (unsigned int i = 0; i < camera->image_width(); ++i) {
      auto color = camera->image()->color(i, j);
      color.x() = new_colorPtr[(j * camera->image_width() + i) * 3 + 0];
      color.y() = new_colorPtr[(j * camera->image_width() + i) * 3 + 1];
      color.z() = new_colorPtr[(j * camera->image_width() + i) * 3 + 2];
      camera->image()->set_color(i, j, color);
    }
  }
  camera->image()->Save("hw4_pt_denoise.exr");
  return 0;
}
