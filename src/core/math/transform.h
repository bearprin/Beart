//
// Created by Bear on 2023/7/6.
//

#pragma once

#include "matrix.h"
#include <enoki/transform.h>
namespace beart {
class Transform {
 public:

  Transform() = default;
  Transform(Mat4f m) : matrix_(std::move(m)) {}

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
    return Ray{TransformPoint(r.ori_), TransformVector(r.dir_), r.depth, r.is_primary_ray_};
  }
  Transform operator*(const Transform &t) const {
    auto m = matrix_ * t.matrix_;
    return Transform{m};
  }

  Mat4f matrix_;
};

inline static
Transform Translate(const Vec3f &v) {
  return enoki::translate<Mat4f, Vec3f>(v);
}
inline static
Transform Scale(const Vec3f &v) {
  return enoki::scale<Mat4f, Vec3f>(v);
}
inline static
Transform Perspective(float fov, float near, float far, float aspect = 1.0f) {
  return enoki::perspective<Mat4f>(fov, near, far, aspect);
}

} // namespace beart
