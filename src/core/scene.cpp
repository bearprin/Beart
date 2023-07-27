#include "scene.h"
#include "light.h"
void beart::Scene::AddLight(beart::Light *light) {
  this->lights_.emplace_back(light);
  light->set_scene(this);
}
const beart::Light *beart::Scene::SampleLight(float u, float *pdf) const {
  assert(lights_.size() > 0);
  std::vector<float> intensities(lights_.size());
  std::vector<float> cdf(lights_.size());
  // save each light's intensity
  std::transform(lights_.begin(), lights_.end(), intensities.begin(), [](const Light *light) {
    return Intensity(light->Power());
  });
  // Compute the CDF
  std::partial_sum(intensities.begin(), intensities.end(), cdf.begin());

  // Normalize the CDF (divide total power)
  for (auto &val : cdf) {
    val /= cdf.back();
  }
  auto it = std::lower_bound(cdf.begin(), cdf.end(), u);
  unsigned index = std::distance(cdf.begin(), it);
  if (pdf) {
    *pdf = cdf[index]; // divide the total power to get the pdf
  }
  assert(index < lights_.size());
  return this->lights_[index];
}
