//
// Created by Bear on 2022/4/12.
//
#include "intersection_info.h"
#include "primitive.h"
#include "light.h"
beart::Spectrum beart::IntersectionInfo::Le(const Vec3f &wo) const {
  if (!primitive_) {
    return Spectrum{0.f, 0.f, 0.f};
  }
  auto light = primitive_->light();
  return light ? light->Le(*this, wo) : Spectrum{0.f, 0.f, 0.f};
}
