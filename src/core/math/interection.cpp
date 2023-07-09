#include "interection.h"
#include "primitive.h"
#include "light.h"
beart::Spectrum beart::SurfaceInterection::Le(const Vec3f &wo) const {
  if (!primitive) {
    return Spectrum{0.f, 0.f, 0.f};
  }
  auto light = primitive->light();
  return light ? light->Le(*this, wo) : Spectrum{0.f, 0.f, 0.f};
}