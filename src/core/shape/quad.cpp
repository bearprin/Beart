//
// Created by Bear on 2023/7/28.
//

#include "quad.h"
beart::Point3f beart::Quad::SampleDirect(const beart::LightSample &ls,
                                         const beart::Point3f &inter_pos,
                                         beart::Vec3f &wi,
                                         beart::Vec3f *n,
                                         float *pdf_solid) const {

  // map [0, 1] to [-1, 1]
  float u = (2 * ls.u_ - 1.0f);
  float v = (2 * ls.v_ - 1.0f);

  // [-1, 1] to world coordinates
  Point3f pos = obj_to_world_.TransformPoint(Point3f(u, v, 0));

  Vec3f L = pos - inter_pos; // inter_point to light (pos)
  float dist = Norm(L);
  Vec3f dir = Normalize(L);

  wi = dir;
  auto world_n = Normalize(obj_to_world_.TransformNormal(quad_normal_));

  float cosine = Dot(-wi, world_n);
  if (pdf_solid) {
    if (cosine <= 0.f) {
      *pdf_solid = 0.f;
    } else {
      // pdf area to pdf solid
      *pdf_solid = dist * dist / (cosine * SurfaceArea());
    }
  }
  if (n) {
    *n = world_n;
  }
  return pos;
}
float beart::Quad::DirectPdf(const beart::Point3f &p, const beart::Vec3f &wi) const {
  return Shape::DirectPdf(p, wi);
}
bool beart::Quad::Intersect(const beart::Ray &ray) const {
  return Intersect(ray, nullptr);
}
bool beart::Quad::Intersect(const beart::Ray &ray, beart::SurfaceInterection *inter) const {
  // transform the ray to object space
  auto temp_r = world_to_obj_ * ray;
  // check if the ray is parallel to the plane
  if (std::abs(temp_r.dir_.z()) < kEpsilon) {
    return false;
  }
  // avoid division by zero
  float t = -temp_r.ori_.z() / temp_r.dir_.z();
  if (t < temp_r.t_min_ || t > temp_r.t_max_) {
    return false;
  }
  auto inter_pos = temp_r(t);
  // check if the intersection is in the quad [-1, 1]
  if (1 < std::abs(inter_pos.x()) || 1 < std::abs(inter_pos.y())) {
    return false;
  }
  // project to the plane
  inter_pos.z() = 0;

  if (inter) {
    inter->hit = true;
    inter->t_curr = t;
    inter->intersect_pos = obj_to_world_.TransformPoint(inter_pos); // back to world space
    inter->view = -ray.dir_;
    inter->Ns = Normalize(obj_to_world_.TransformNormal(quad_normal_)); // back to world space
    inter->Ng = inter->Ns;  // Ng eq Ns
    Vec3f v1;
    Vec3f v2;
    CoordinateSystem(inter->Ns, &v1, &v2); // build local coordinate system with shading normal
    inter->tangent = v1;
  }
  return true;
}
float beart::Quad::SurfaceArea() const {
  return width_ * height_; // directly compute the real surface area
}
float beart::Quad::Volume() const {
  return 0;
}
const beart::AABB &beart::Quad::bbox() const {
  if (!bbox_) {
    bbox_ = std::make_unique<beart::AABB>();
    bbox_->Union((obj_to_world_.TransformPoint({-1.f, -1.f, 0.f})));
    bbox_->Union((obj_to_world_.TransformPoint({1.f, -1.f, 0.f})));
    bbox_->Union((obj_to_world_.TransformPoint({-1.f, 1.f, 0.f})));
    bbox_->Union((obj_to_world_.TransformPoint({1.f, 1.f, 0.f})));
  }
  return *bbox_;
}
