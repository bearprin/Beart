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

#include "zlib.h"
#define TINYEXR_USE_MINIZ 0
#define TINYEXR_IMPLEMENTATION
#include <tinyexr.h>
bool SaveEXR(float *rgb, int width, int height, const char *outfilename) {

  EXRHeader header;
  InitEXRHeader(&header);

  EXRImage image;
  InitEXRImage(&image);

  image.num_channels = 3;

  std::vector<float> images[3];
  images[0].resize(width * height);
  images[1].resize(width * height);
  images[2].resize(width * height);

  // Split RGBRGBRGB... into R, G and B layer
  for (int i = 0; i < width * height; i++) {
    images[0][i] = rgb[3 * i + 0];
    images[1][i] = rgb[3 * i + 1];
    images[2][i] = rgb[3 * i + 2];
  }

  float *image_ptr[3];
  image_ptr[0] = &(images[2].at(0)); // B
  image_ptr[1] = &(images[1].at(0)); // G
  image_ptr[2] = &(images[0].at(0)); // R

  image.images = (unsigned char **) image_ptr;
  image.width = width;
  image.height = height;

  header.num_channels = 3;
  header.channels = (EXRChannelInfo *) malloc(sizeof(EXRChannelInfo) * header.num_channels);
  // Must be (A)BGR order, since most of EXR viewers expect this channel order.
  strncpy(header.channels[0].name, "B", 255);
  header.channels[0].name[strlen("B")] = '\0';
  strncpy(header.channels[1].name, "G", 255);
  header.channels[1].name[strlen("G")] = '\0';
  strncpy(header.channels[2].name, "R", 255);
  header.channels[2].name[strlen("R")] = '\0';

  header.pixel_types = (int *) malloc(sizeof(int) * header.num_channels);
  header.requested_pixel_types = (int *) malloc(sizeof(int) * header.num_channels);
  for (int i = 0; i < header.num_channels; i++) {
    header.pixel_types[i] = TINYEXR_PIXELTYPE_FLOAT; // pixel type of input image
    header.requested_pixel_types[i] = TINYEXR_PIXELTYPE_HALF; // pixel type of output image to be stored in .EXR
  }
  const char *err = NULL; // or nullptr in C++11 or later.
  int ret = SaveEXRImageToFile(&image, &header, outfilename, &err);
  if (ret != TINYEXR_SUCCESS) {
    fprintf(stderr, "Save EXR err: %s\n", err);
    FreeEXRErrorMessage(err); // free's buffer for an error message
    return ret;
  }
  printf("Saved exr file. [ %s ] \n", outfilename);
  free(rgb);
  free(header.channels);
  free(header.pixel_types);
  free(header.requested_pixel_types);
}
int main() {
  auto phong = std::make_unique<beart::Phong>(beart::Spectrum{0.5, 0.5, 0.5}, beart::Spectrum{0.2, 0.2, 0.2}, 30.0);
//  auto phong = std::make_unique<beart::NormalMaterial>();

//  std::unique_ptr<beart::Shape> sphere1 = std::make_unique<beart::Sphere>(beart::Vec3f{-0.2, 0.2, -3.f}, 0.1);
//  auto sphere1_prim = std::make_unique<beart::Primitive>(sphere1.get(), phong.get());
//  std::unique_ptr<beart::Shape> sphere2 = std::make_unique<beart::Sphere>(beart::Vec3f{-0.1f, -0.1, -3.f}, 0.2f);
//  auto sphere2_prim = std::make_unique<beart::Primitive>(sphere2.get(), phong.get());
  std::unique_ptr<beart::Shape> sphere3 = std::make_unique<beart::Sphere>(beart::Vec3f{0, 0, 0}, 0.1f);
  auto sphere3_prim = std::make_unique<beart::Primitive>(sphere3.get(), phong.get());
  std::unique_ptr<beart::Shape> sphere4 = std::make_unique<beart::Sphere>(beart::Vec3f{0, -100.5f, -5.f}, 100.5f);
  auto sphere4_prim = std::make_unique<beart::Primitive>(sphere4.get(), phong.get());
//  auto camera = std::make_unique<const beart::PerspectiveCamera>(400,
//                                                                 300,
//                                                                 beart::Vec3f{0, 0, -1},
//                                                                 beart::Vec3f{0, 0.1, 0.1},
//                                                                 beart::Vec3f{0, 1, 0},
//                                                                 30.0);
  auto camera = std::make_unique<const beart::PerspectiveCamera>(400,
                                                                 400,
                                                                 beart::Vec3f{0, 0, 0},
                                                                 beart::Vec3f{0, 0.5, -1},
                                                                 beart::Vec3f{0, 1, 0},
                                                                 60.0);
//  auto point_light =
//      std::make_unique<beart::PointLight>(beart::Vec3f{0.2f, 0.1f, -2}, beart::Spectrum{1, 1, 1});
  auto point_light =
      std::make_unique<beart::PointLight>(beart::Vec3f{0.2f, 0.2f, 0}, beart::Spectrum{1, 1, 1});
  auto distant_light =
      std::make_unique<beart::DirectionalLight>(beart::Vec3f{-0.3f, -1, 0}, beart::Spectrum{1, 1, 1});
  auto integrator = beart::Direct{};

  beart::Scene scene;
//  scene.AddPrimitive(sphere1_prim.get());
//  scene.AddPrimitive(sphere2_prim.get());
  scene.AddPrimitive(sphere3_prim.get());
  scene.AddPrimitive(sphere4_prim.get());
  scene.AddLight(point_light.get());
//  scene.AddLight(distant_light.get());
  scene.set_camera(camera.get());
  scene.Prepare();
  auto *rgb = new float[camera->image_width() * camera->image_height() * 3];
  for (unsigned j = 0; j < scene.camera()->image_height(); ++j) {
    for (unsigned int i = 0; i < scene.camera()->image_width(); ++i) {
      // TODO: generate sample by i, j
      auto ps = beart::PixelSample{0.f, 0.f};
      beart::Ray r = scene.camera()->GenerateRay(i, j, ps);
      auto nor = integrator.Li(r, scene, ps);
      rgb[3 * (j * scene.camera()->image_width() + i) + 0] = nor.x();
      rgb[3 * (j * scene.camera()->image_width() + i) + 1] = nor.y();
      rgb[3 * (j * scene.camera()->image_width() + i) + 2] = nor.z();
    }
  }
  SaveEXR(rgb, scene.camera()->image_width(), scene.camera()->image_height(), "sphere_normal.exr");
  return 0;
}
