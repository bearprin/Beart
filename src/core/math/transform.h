//
// Created by Bear on 2021/12/3.
//

#pragma once

#include "mat.h"
namespace beart {

using Transform = Mat4f;
static Vec4f TransformPoint(const Transform &trans, const Vec4f &point) {
  Vec4f res = (trans * point);
  if (res.w() == 1) {
    return res;
  }
  return res / res.w();
}
static Vec3f TransformPoint(const Transform &trans, const Vec3f &point) {
  Vec4f p{point.x(), point.y(), point.z(), 1.0f};
  Vec4f res = (trans * p);
  if (res.w() == 1) {
    return {res.x(), res.y(), res.z()};
  }
  return {res.x() / res.w(), res.y() / res.w(), res.z() / res.w()};
}
static Vec3f TransformVector(const Transform &trans, const Vec3f &v) {
  return trans.topLeftCorner<3, 3>() * v;
}
static Transform Scale(const float &x, const float &y, const float &z) {
  Transform scale{
      {x, 0.0f, 0.0f, 0.0f},
      {0.0f, y, 0.0f, 0.0f},
      {0.0f, 0.0f, z, 0.0f},
      {0.0f, 0.0f, 0.0f, 1.0f},
  };
  return scale;
}
static Transform Translate(const float &x, const float &y, const float &z) {
  Transform translate{
      {1.0f, 0.0f, 0.0f, x},
      {0.0f, 1.0f, 0.0f, y},
      {0.0f, 0.0f, 1.0f, z},
      {0.0f, 0.0f, 0.0f, 1.0f},
  };
  return translate;
}
static Transform Translate(const Vec3f &v) {
  Transform translate{
      {1.0f, 0.0f, 0.0f, v.x()},
      {0.0f, 1.0f, 0.0f, v.y()},
      {0.0f, 0.0f, 1.0f, v.z()},
      {0.0f, 0.0f, 0.0f, 1.0f},
  };
  return translate;
}
static Transform LookAt(const Vec3f &pos, const Vec3f &up, const Vec3f &dir) {
  Vec3f z = dir.normalized();
  Vec3f x = Normalize(Cross(up, z));
  Vec3f y = Normalize(Cross(z, x));

  Transform camera_2_world{
      {x.x(), y.x(), z.x(), pos.x()},
      {x.y(), y.y(), z.y(), pos.y()},
      {x.z(), y.z(), z.z(), pos.z()},
      {0.0f, 0.0f, 0.0f, 1.0f}
  };
  return camera_2_world;
}
static Transform Perspective(const float &scale_x, const float &scale_y, const float &near, const float &far) {
  Transform camera2screen_perspective{{scale_x, 0.0f, 0.0f, 0.0f},
                                      {0.0f, scale_y, 0.0f, 0.0f},
                                      {0.0f, 0.0f, far / (far - near), -far * near / (far - near)},
                                      {0.0f, 0.0f, 1.0f, 0.0f}};
  return camera2screen_perspective;
}
}