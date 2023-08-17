//
// Created by Bear on 2023/8/15.
//

#include "scene_parser.h"

#include "factory.h"
#include "integrator.h"
#include "random_sampler.h"
#include "light.h"
#include "camera.h"
#include "scene.h"
#include "primitive.h"
#include "diffuse.h"

void beart::SceneParser::Parse_(const beart::json &j) {
  spdlog::info("Parsing scene ...");

  if (j.contains("camera")) {
    camera_ = BeartFactory<Camera>::CreateUnique(j["camera"]);
    spdlog::info("camera: {}", j["camera"].dump(4));
  } else {
    spdlog::error("No camera specified in scene!");
  }

  if (j.contains("integrator")) {
    integrator_ = BeartFactory<Integrator>::CreateUnique(j["integrator"]);
    spdlog::info("integrator: {}", j["integrator"].dump(4));
  } else {
    spdlog::error("No integrator specified in scene!");
  }

  // read number of samples to take per pixel
  //
  if (j.contains("sampler")) {
    sampler_ = BeartFactory<Sampler>::CreateUnique(j["sampler"]);
    spdlog::info("sampler: {}", j["sampler"].dump(4));
  } else {
    sampler_ = std::make_unique<RandomSampler>();
  }

  if (j.contains("accelerator")) {
    accelerator_ = BeartFactory<Accelerator>::CreateUnique(j["accelerator"]);
    spdlog::info("accelerator: {}", j["accelerator"].dump(4));
  } else {
    accelerator_ = std::make_unique<Accelerator>();
  }
//  //
//  // parse scene sky light TODO
  Spectrum background(0.f, 0.f, 0.f);
  if (j.contains("background")) {
    background = j.value("background", Spectrum{0.f, 0.f, 0.f});
  }

  auto check_key = [](const std::string &key, const std::string &parent, const json &j) {
    try {
      return j.at(key).get<std::string>();
    }
    catch (...) {
      spdlog::error("Missing '{}' on '{}' specification:\n{}", key, parent, j.dump(4));
    }
  };
  if (j.contains("materials")) {
    for (auto &m : j["materials"]) {
      auto material = BeartFactory<Bxdf>::CreateShared(m);
      BeartFactory<Bxdf>::RegisterInstance(check_key("name", "materials", m),
                                           material);  // save material ptr to registry
      spdlog::info("{}: {}, {}", m["name"], m["type"], m.dump(4));
      spdlog::info("registering material with name {}", check_key("name", "materials", m));
    }
  }

  if (j.contains("lights")) {
    for (auto &l : j["lights"]) {
      auto light = BeartFactory<Light>::CreateUnique(l);
      if (light->light_type() == LightType::kArea && light->shape()) {
        std::unique_ptr<Primitive> primitive;
        if (l.contains("material")) {
          auto m = BeartFactory<Bxdf>::Find(l);
          primitive = std::make_unique<Primitive>(light->shape(), light.get(), m);
        } else {
          primitive = std::make_unique<Primitive>(light->shape(), light.get());
        }
        primitives_.emplace_back(std::move(primitive));
      }
      lights_.emplace_back(std::move(light));
      spdlog::info("{}: {}", l["type"], l.dump(4));
    }
  }


  //
  // parse surfaces
  //
  if (j.contains("surfaces")) {
    for (auto &s : j["surfaces"]) {
      auto shape = BeartFactory<Shape>::CreateShared(s);
      // find material
      std::shared_ptr<Bxdf> m = nullptr;
      if (s.contains("material")) {
        m = BeartFactory<Bxdf>::Find(s);
      } else {
        m = std::make_shared<Diffuse>();
      }
      auto primitive = std::make_unique<Primitive>(shape.get(), m);
      shapes_.emplace_back(std::move(shape));
      primitives_.emplace_back(std::move(primitive));
      spdlog::info("{}: {}", s["type"], s.dump(4));
    }
  }





  // set of all fields we'd expect to see at the top level of a darts scene
  std::set<std::string> toplevel_fields{"integrator", "materials", "surfaces",
                                        "accelerator", "camera", "sampler", "lights", "background"};

  // now loop through all keys in the json file to see if there are any that we don't recognize
  for (auto it = j.begin(); it != j.end(); ++it) {
    if (toplevel_fields.count(it.key()) == 0) {
      spdlog::warn("Unrecognized field '{}' in scene file.", it.key());
    }
  }
  // init scene
  scene_ = std::make_unique<Scene>(std::move(accelerator_));
  scene_->set_background(background);
  for (const auto &primitive : primitives_) {
    scene_->AddPrimitive(primitive.get());
  }
  // last step: add lights to scene for the bbox of the scene after addprimitives
  for (const auto &light : lights_) {
    scene_->AddLight(light.get());
    if (light->light_type() == LightType::kConstant) {
      auto primitive = std::make_unique<Primitive>(light->shape(), light.get());
      scene_->AddPrimitive(primitive.get());
      primitives_.emplace_back(std::move(primitive));
    }
  }
  scene_->set_camera(camera_.get());
  scene_->Prepare();

  spdlog::info("done parsing scene.");
}
beart::SceneParser::SceneParser(const std::filesystem::path &path) {

  // modified from darts
  // open file
  std::ifstream stream(path.string(), std::ifstream::in);
  if (!stream.good()) {
    spdlog::critical("Cannot open file: {}", path.string());
  }

  json j = json::parse(stream,
      /* callback */ nullptr,
      /* allow exceptions */ true,
      /* ignore_comments */ true);
  auto current_path = std::filesystem::current_path();
  fs::current_path(path.parent_path());
  Parse_(j);
  fs::current_path(current_path);
}
beart::SceneParser::~SceneParser() = default;
