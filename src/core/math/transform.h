//
// Created by Bear on 2023/7/6.
//

#pragma once

#include "matrix.h"
#include "common.h"
#include "ray.h"

#include <enoki/transform.h>
namespace beart {
class Transform {
 public:

  Transform() : matrix_(enoki::identity<Mat4f>()) {}
  explicit Transform(Mat4f m) : matrix_(std::move(m)) {}
  Point3f TransformPoint(const Point3f &p) const {
    Point4f p_new = Point4f{p.x(), p.y(), p.z(), 1.0};
    auto res = matrix_ * p_new;
    if (LIKELY(res.w() == 1.0f)) {
      return Point3f{res.x(), res.y(), res.z()};
    }
    return Point3f{res.x(), res.y(), res.z()} / res.w();
  }
  Vec3f TransformVector(const Vec3f &v) const {
    auto new_mat = Mat4ftoMat3f(matrix_);
    auto res = new_mat * v;
    return Vec3f{res.x(), res.y(), res.z()};
  }
  Vec3f TransformNormal(const Vec3f &n) const {
    return Transform{inverse_transpose(matrix_)}.TransformVector(n);
  }

  Ray operator*(const Ray &r) const {
    return Ray{TransformPoint(r.ori_), TransformVector(r.dir_), r.depth_, r.is_primary_ray_};
  }
  Transform operator*(const Transform &t) const {
    return Transform{matrix_ * t.matrix_};
  }
  Point3f operator*(const Point3f &p) const {
    return TransformPoint(p);
  }

  Mat4f matrix_;
  static Transform Identity() noexcept {
    return Transform{enoki::identity<Mat4f>()};
  }
};
inline static
Transform Inverse(const Transform &t) {
  return Transform{Inverse(t.matrix_)};
}
inline static
Transform Translate(const Vec3f &v) {
  return Transform{enoki::translate<Mat4f, Vec3f>(v)};
}
inline static
Transform Scale(const Vec3f &v) {
  return Transform{enoki::scale<Mat4f, Vec3f>(v)};
}
inline static
Transform Perspective(float fov, float near, float far, float aspect = 1.0f) {
  /* Project vectors in camera space onto a plane at z=1:
  *
  *  xProj = x / z
  *  yProj = y / z
  *  zProj = (far * (z - near)) / (z * (far-near))
  *
  *  Camera-space depths are not mapped linearly!
  */
  float recip = 1.0f / (far - near);
  float cot = 1.0 / std::tanf(DegToRad(fov) * 0.5f);

  Mat4f trafo = enoki::diag<Mat4f>(
      enoki::column_t<Mat4f>(cot / aspect, cot, far * recip, 0));

  trafo(2, 3) = -2.f * near * far * recip;
  trafo(3, 2) = 1.f;
  return Transform{trafo};
}
inline static
Transform LookAt(const Vec3f &camera_pos, const Vec3f &target, const Vec3f &up) {
  Vec3f dir = Normalize(target - camera_pos);
  Vec3f left = Normalize(Cross(up, dir));
  Vec3f new_up = Normalize(Cross(dir, left));

  Mat4f res = Mat4f::from_cols(
      enoki::concat(left, Mat4f::Scalar(0)),
      enoki::concat(new_up, Mat4f::Scalar(0)),
      enoki::concat(dir, Mat4f::Scalar(0)),
      enoki::concat(camera_pos, Mat4f::Scalar(1))
  );
  return Transform{res};
}

} // namespace beart
