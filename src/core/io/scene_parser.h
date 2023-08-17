//
// Created by Bear on 2023/8/15.
//

#pragma once

#include "common.h"
#include "json_serializable.h"
namespace beart {
class SceneParser {
 public:
  explicit SceneParser(const fs::path &path);
  ~SceneParser();

  [[nodiscard]] const Scene *scene() const {
    return scene_.get();
  }
  [[nodiscard]] const Integrator *integrator() const {
    return integrator_.get();
  }
  [[nodiscard]] const Sampler *sampler() const {
    return sampler_.get();
  }
  [[nodiscard]] const Camera *camera() const {
    return camera_.get();
  }
 private:
  void Parse_(const json &j);

  std::unique_ptr<Scene> scene_;
  std::unique_ptr<Accelerator> accelerator_;
  std::unique_ptr<Integrator> integrator_;
  std::unique_ptr<Camera> camera_;
  std::unique_ptr<Sampler> sampler_;

  std::vector<std::unique_ptr<Light>> lights_;
  std::vector<std::shared_ptr<Shape>> shapes_;
  std::vector<std::unique_ptr<Primitive>> primitives_;
};
}
