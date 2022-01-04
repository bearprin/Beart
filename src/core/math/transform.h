//
// Created by Bear on 2021/12/3.
//

#pragma once

#include "mat.h"
namespace beart {

using Transform = Mat4f;
static Vec3f TransformPoint(const Transform &trans, const Vec3f &point) {
  Vec4f p{point.x(), point.y(), point.z(), 1.0f};
  Vec4f res = (trans * p);
  return std::move(Vec3f{res.x(), res.y(), res.z()});
}
static Transform Scale(const float &x, const float &y, const float &z) {
  Transform scale{
      {x, 0.0f, 0.0f, 0.0f},
      {0.0f, y, 0.0f, 0.0f},
      {0.0f, 0.0f, z, 0.0f},
      {0.0f, 0.0f, 0.0f, 1.0f},
  };
  return std::move(scale);
}
static Transform Translate(const float &x, const float &y, const float &z) {
  Transform translate{
      {1.0f, 0.0f, 0.0f, x},
      {0.0f, 1.0f, 0.0f, y},
      {0.0f, 0.0f, 1.0f, z},
      {0.0f, 0.0f, 0.0f, 1.0f},
  };
  return std::move(translate);
}
static Transform LookAt(const Vec3f &pos, const Vec3f &up, const Vec3f &dir) {
  Vec3f z = dir.normalized();
  Vec3f x = Cross(z, up).normalized();
  Vec3f y = Cross(x, z).normalized();

  Transform camera_2_world{
      {x.x(), y.x(), z.x(), pos.x()},
      {x.y(), y.y(), z.y(), pos.y()},
      {x.z(), y.z(), z.z(), pos.z()},
      {0.0f, 0.0f, 0.0f, 1.0f}
  };
  return std::move(camera_2_world);
}
static Transform Perspective(const float &scale_x, const float &scale_y, const float &near, const float &far) {
  Transform camera2screen_perspective{{scale_x, 0.0f, 0.0f, 0.0f},
                                      {0.0f, scale_y, 0.0f, 0.0f},
                                      {0.0f, 0.0f, far / (far - near), -far * near / (far - near)},
                                      {0.0f, 0.0f, 1.0f, 0.0f}};
  return std::move(camera2screen_perspective);
}
}