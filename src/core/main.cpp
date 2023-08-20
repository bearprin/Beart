#include "scene_parser.h"
#include "camera.h"
#include "sampler.h"
#include "integrator.h"

#include <spdlog/fmt/chrono.h>
#include <CLI/CLI.hpp>
#include <nanothread/nanothread.h>
#ifdef BERT_ENABLE_OIDN
#include <OpenImageDenoise/oidn.hpp>
#endif
namespace dr = drjit;
int main(int argc, char **argv) {
  CLI::App app{"Beart Ray tracing", "Beart"};
  std::string scene_file;
  std::string output_path;
  size_t thread_num{1};
  app.add_option("-s, --scene", scene_file, "The filename of the JSON scenefile to load")->required();
  app.add_option("-o, --outfile", output_path, "Specify the output image path");
  app.add_option("-t, --threads", thread_num, "Number of threads to use in the thread pool")
      ->check(CLI::NonNegativeNumber)->default_val(1);
  CLI11_PARSE(app, argc, argv)

  spdlog::info("Parse scene from {}", scene_file);
  beart::SceneParser parser(scene_file);

  auto scene = parser.scene();
  auto integrator = parser.integrator();
  auto sampler = parser.sampler();
  auto camera = parser.camera();

  uint sample_count = sampler->sample_count();
  spdlog::info("Sample count: {}", sample_count);
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
              L += integrator->Li(r, *scene, *sampler, &normal, &albendo);
            }
            L = L / sample_count;
            normal = normal / sample_count;
            albendo = albendo / sample_count;
            camera->image()->set_color(i, j, L);
            camera->normal()->set_color(i, j, normal);
          }
        });
  }
  // default output path
  if (output_path.empty()) {
    std::time_t t = std::time(nullptr);
    std::string time_string = fmt::format("{:%Y-%m-%d-%H-%M-%S}", fmt::localtime(t));

    // build a dir called output to save the output
    std::string dir = "../output/";
    std::filesystem::create_directories(dir);
    std::string scene_name = scene_file.substr(scene_file.find_last_of('/') + 1);
    scene_name = scene_name.substr(0, scene_name.find_last_of('.')) + "_" + time_string + ".";
    output_path = dir + scene_name + "exr";
  }
  spdlog::info("Save image to {}", output_path);
  camera->image()->Save(output_path);
  spdlog::info("Save normal to normal.exr");
  camera->normal()->Save(fmt::format("{}_normal.exr", output_path.substr(0, output_path.find_last_of('.'))));

#ifdef BERT_ENABLE_OIDN
spdlog::info("Denoise image");
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

  if (const char *errorMessage; device.getError(errorMessage) != oidn::Error::None)
    std::cout << "Error: " << errorMessage << std::endl;
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
  auto denoise_path = output_path.substr(0, output_path.find_last_of('.')) + "_denoise.exr";
  spdlog::info("Save denoise image to {}", denoise_path);
  camera->image()->Save(denoise_path);
  return 0;
#endif
}
