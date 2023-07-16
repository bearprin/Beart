#include "interection.h"
#include "primitive.h"
#include "light.h"
beart::Spectrum beart::SurfaceInterection::Le(const beart::Vec3f &wo, float *pdf_area, float *pdf_solid) const {
  if (!primitive) {
    return Spectrum{0.f, 0.f, 0.f};
  }
  auto light = primitive->light();
  return light ? light->Le(*this, wo, pdf_area, pdf_solid) : Spectrum{0.f, 0.f, 0.f};
}
