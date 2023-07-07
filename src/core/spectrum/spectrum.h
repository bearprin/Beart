//
// Created by Bear on 2021/12/4.
//

#pragma once

#include "vec.h"
namespace beart {

using RGBSpectrum = Vec3f;
using RGBASpectrum = Vec4f;

/// Convert from linear RGB to sRGB
inline static
RGBSpectrum LinearRGBTosRGB(const RGBSpectrum &l) {
  // use select to translate
  return enoki::select(l <= 0.0031308f, l * 12.92f, 1.055f * enoki::pow(l, 1.f / 2.4f) - 0.055f);
}
inline static
RGBSpectrum sRGBToLinearRGB(const RGBSpectrum &s) {
  return enoki::select(s <= 0.04045f, s / 12.92f, enoki::pow((s + 0.055f) / 1.055f, 2.4f));
}

} // namespace beart
