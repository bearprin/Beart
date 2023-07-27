//
// Created by Bear on 2023/07/4.
//

#pragma once

#include "vec.h"
namespace beart {

using RGBSpectrum = Vec3f;
using RGBASpectrum = Vec4f;

using Spectrum = RGBSpectrum;  // TODO: add spectrum type

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
inline static
bool IsBlack(const RGBSpectrum &s) {
  return enoki::all(enoki::eq(s, 0.f));
}
inline static float Intensity(const RGBSpectrum &s) {
  constexpr float YWeight[3] = {0.212671f, 0.715160f, 0.072169f}; // ITU-R BT.709
  return YWeight[0] * s.data()[0] + YWeight[1] * s.data()[1] + YWeight[2] * s.data()[2];
}
} // namespace beart
