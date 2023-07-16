#include "scene.h"
#include "light.h"
void beart::Scene::AddLight(beart::Light *light) {
  this->lights_.emplace_back(light);
  light->set_scene(this);
}
