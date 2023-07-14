//
// Created by Bear on 2023/7/12.
//

#include "triangle.h"
#include "triangle_mesh.h"
bool beart::Triangle::Intersect(const beart::Ray &ray, beart::SurfaceInterection *inter) const {
  // Moller-Trumbore algorithm <tt>http://www.acm.org/jgt/papers/MollerTrumbore97/code.html</tt>.
  // compute the edge vectors
  auto e1 = p_1_ - p_0_;
  auto e2 = p_2_ - p_0_;
  // compute the determinant
  auto h = Cross(ray.dir_, e2);
  auto det = Dot(e1, h);
  // if determinant is near zero, ray lies in plane of triangle
  if (det > -kEpsilon && det < kEpsilon) {
    return false;
  }
  float inv_det = 1.f / det;
  // calculate distance from p_0_ to ray origin
  auto s = ray.ori_ - p_0_;
  // calculate u parameter and test bound
  float u = Dot(s, h) * inv_det;
  // the intersection lies outside of the triangle
  if (u < 0.f || u > 1.f) {
    return false;
  }
  // prepare to test v parameter
  auto q = Cross(s, e1);
  // calculate v parameter and test bound
  auto v = Dot(ray.dir_, q) * inv_det;
  // the intersection lies outside of the triangle
  if (v < 0.f || u + v > 1.f) {
    return false;
  }
  // calculate t, ray intersects triangle
  float t = Dot(e2, q) * inv_det;
  if (t < kEpsilon) {
    return false;
  }
  if (t > ray.t_max_ || t < ray.t_min_) {
    return false;
  }
  if (inter) {
    inter->t_curr = t;
    inter->intersect_pos = ray(t);
    inter->view = -ray.dir_;
    inter->Ng = Normalize(Cross(e1, e2)); // geometric normal
    if (mesh_ && !mesh_->vn_.empty()) { // if the triangle is from a mesh
      inter->Ns = (1 - u - v) * mesh_->vn_[mesh_->Fv_[tri_index_][0]] + u * mesh_->vn_[mesh_->Fv_[tri_index_][1]] +
          v * mesh_->vn_[mesh_->Fv_[tri_index_][2]];
    } else {
      inter->Ns = inter->Ng;
    }
    Vec3f v1;
    Vec3f v2;
    CoordinateSystem(inter->Ns, &v1, &v2); // build local coordinate system with shading normal
    inter->tangent = v1;
    inter->hit = true;
  }
  return true;
}
bool beart::Triangle::Intersect(const beart::Ray &ray) const {
  return Intersect(ray, nullptr);
}

float beart::Triangle::SurfaceArea() const {
  // compute the area of the triangle
  auto e1 = p_1_ - p_0_;
  auto e2 = p_2_ - p_0_;
  return Norm(Cross(e1, e2)) * 0.5f;  // 0.5 * |e1 x e2|
}
float beart::Triangle::Volume() const {
  return 0;
}
const beart::AABB &beart::Triangle::bbox() const {
  if (!bbox_) {
    bbox_ = std::make_unique<AABB>();
    bbox_->Union(p_0_);
    bbox_->Union(p_1_);
    bbox_->Union(p_2_);
  }
  return *bbox_;
}
