
#include "vec.h"
#include <iostream>
namespace beart {
///
/// \param v1
/// \param ng
/// \return
static inline bool IsSameSide(const Vec3f &v1, const Vec3f &ng) {
  std::cout << Dot(v1, ng) << std::endl;
  return Dot(v1, ng) > 0.f;
}
///
/// \param v1
/// \param v2
/// \return
static inline bool IsSameHemisphere(const Vec3f &v1, const Vec3f &v2, const Vec3f &ng) {
  return IsSameSide(v1, ng) * IsSameSide(v2, ng) > 0.f;
}
/// @brief Compute the cosine of the angle between two vectors.
/// \param v1
/// \param v2
/// \return
static inline float SafeDot(const Vec3f &v1, const Vec3f &v2) {
  return std::clamp(Dot(v1, v2), 0.0f, 1.0f);
}
/// @brief Compute the cosine of the angle between two vectors.
/// shading coordinate optimization (ns as axis y)
/// \param v
/// \param ns
/// \return
static inline float AbsCosTheta(const Vec3f &v) {
  return fabsf(v.y());
}
///
/// shading coordinate optimization (ns as axis y)
/// \param v
/// \return
static inline beart::Vec3f Reflect(const Vec3f &v) {
  return {-v.x(), v.y(), -v.z()};
}
}