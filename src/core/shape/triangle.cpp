//
// Created by Bear on 2023/7/12.
//

#include "triangle.h"
#include "triangle_mesh.h"
bool beart::Triangle::Intersect(const beart::Ray &ray, beart::SurfaceInterection *inter) const {
  // (PBRT style)
  // transform the vertices from world space to ray coordinate
  // step 0 : translate the vertices with ray origin
  auto p_0_t = p_0_ - ray.ori_;
  auto p_1_t = p_1_ - ray.ori_;
  auto p_2_t = p_2_ - ray.ori_;

  // step 1 : pick the major axis to avoid dividing by zero in the sheering pass.
  //          by picking the major axis, we can also make sure we sheer as little as possible
  int kz = MaxDim(Abs(ray.dir_));
  int kx = (kz + 1) % 3;
  int ky = (kx + 1) % 3;
  Vec3f d = Permute(ray.dir_, kx, ky, kz);
  p_0_t = Permute(p_0_t, kx, ky, kz);
  p_1_t = Permute(p_1_t, kx, ky, kz);
  p_2_t = Permute(p_2_t, kx, ky, kz);

  // step 2 : sheer the vertices so that the ray direction points to ( 0 , 0 , 1)
  float sx = -d.x() / d.z();
  float sy = -d.y() / d.z();
  float sz = 1.f / d.z();
  p_0_t.x() += sx * p_0_t.z();
  p_0_t.y() += sy * p_0_t.z();
  p_1_t.x() += sx * p_1_t.z();
  p_1_t.y() += sy * p_1_t.z();
  p_2_t.x() += sx * p_2_t.z();
  p_2_t.y() += sy * p_2_t.z();

  // compute the edge functions

  float e0 = p_1_t.x() * p_2_t.y() - p_1_t.y() * p_2_t.x();
  float e1 = p_2_t.x() * p_0_t.y() - p_2_t.y() * p_0_t.x();
  float e2 = p_0_t.x() * p_1_t.y() - p_0_t.y() * p_1_t.x();

  // fall back to double precision for better accuracy at some performance cost
  if (UNLIKELY(e0 == 0.0f || e1 == 0.0f || e2 == 0.0f)) {
    e0 = (float) ((double) p_1_t.x() * (double) p_2_t.y() - (double) p_1_t.y() * (double) p_2_t.x());
    e1 = (float) ((double) p_2_t.x() * (double) p_0_t.y() - (double) p_2_t.y() * (double) p_0_t.x());
    e2 = (float) ((double) p_0_t.x() * (double) p_1_t.y() - (double) p_0_t.y() * (double) p_1_t.x());
  }

  if ((e0 < 0 || e1 < 0 || e2 < 0) && (e0 > 0 || e1 > 0 || e2 > 0)) {
    return false;
  }
  float det = e0 + e1 + e2;
  if (det == 0) {
    return false;
  }
  p_0_t.z() *= sz;
  p_1_t.z() *= sz;
  p_2_t.z() *= sz;
  float t_scaled = e0 * p_0_t.z() + e1 * p_1_t.z() + e2 * p_2_t.z();
  if (det < 0 && (t_scaled >= 0 || t_scaled < ray.t_max_ * det)) {
    return false;
  } else if (det > 0 && (t_scaled <= 0 || t_scaled > ray.t_max_ * det)) {
    return false;
  }
  float inv_det = 1 / det;
  float b0 = e0 * inv_det;
  float b1 = e1 * inv_det;
  float b2 = e2 * inv_det;
  float t = t_scaled * inv_det;

  // naive ray-triangle intersection, errors with float precision sometimes
//  constexpr float kTriEpsilon = 1e-8f;
  // compute the edge vectors
//  auto e1 = p_1_ - p_0_;
//  auto e2 = p_2_ - p_0_;
  // compute the determinant
//  auto h = Cross(ray.dir_, e2);
//  auto det = Dot(e1, h);
  // if determinant is near zero, ray lies in plane of triangle
//  if (det > -kTriEpsilon && det < kTriEpsilon) {
//    return false;
//  }
//  float inv_det = 1.f / det;
  // calculate distance from p_0_ to ray origin
//  auto s = ray.ori_ - p_0_;
  // calculate u parameter and test bound
//  float u = Dot(s, h) * inv_det;
  // the intersection lies outside of the triangle
//  if (u < 0.f || u > 1.f) {
//    return false;
//  }
  // prepare to test v parameter
//  auto q = Cross(s, e1);
  // calculate v parameter and test bound
//  auto v = Dot(ray.dir_, q) * inv_det;
  // the intersection lies outside of the triangle
//  if (v < 0.f || u + v > 1.f) {
//    return false;
//  }
  // calculate t, ray intersects triangle
//  float t = Dot(e2, q) * inv_det;
  if (t > ray.t_max_ || t < ray.t_min_) {
    return false;
  }
  if (inter) {
    inter->t_curr = t;
    inter->intersect_pos = ray(t);
    inter->view = -ray.dir_;
    inter->Ng = Normalize(Cross(p_1_ - p_0_, p_2_ - p_0_)); // geometric normal
    if (mesh_ && !mesh_->vn_.empty()) { // if the triangle is from a mesh
//            inter->Ns = (1 - u - v) * mesh_->vn_[mesh_->Fv_[tri_index_][0]] + u * mesh_->vn_[mesh_->Fv_[tri_index_][1]] +
//          v * mesh_->vn_[mesh_->Fv_[tri_index_][2]];
      inter->Ns = b0 * mesh_->vn_[mesh_->Fv_[tri_index_][0]] + b1 * mesh_->vn_[mesh_->Fv_[tri_index_][1]] +
          b2 * mesh_->vn_[mesh_->Fv_[tri_index_][2]];
    } else {
      inter->Ns = inter->Ng;
    }
    // fix orientation of Ng
    inter->Ng = FaceForward(inter->Ng, inter->Ns);
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
