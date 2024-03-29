//
// Created by Bear on 2023/7/12.
//

#pragma once

#include "shape.h"
#include "triangle_mesh.h"
#include <utility>

namespace beart {
class Triangle : public Shape {
 public:
  Triangle(Vec3f p_0, Vec3f p_1, Vec3f p_2) : p_0_(std::move(p_0)), p_1_(std::move(p_1)), p_2_(std::move(p_2)) {

  }
  Triangle(Vec3f p_0, Vec3f p_1, Vec3f p_2, Transform obj_to_world)
      : p_0_(std::move(p_0)), p_1_(std::move(p_1)), p_2_(std::move(p_2)), Shape(obj_to_world) {

  }
  Triangle(TriangleMesh *mesh, unsigned int tri_index)
      : mesh_(mesh), tri_index_(tri_index) {
    p_0_ = mesh_->vs_[mesh_->Fv_[tri_index_][0]];
    p_1_ = mesh_->vs_[mesh_->Fv_[tri_index_][1]];
    p_2_ = mesh_->vs_[mesh_->Fv_[tri_index_][2]];
  }
  Point3f SampleDirect(const LightSample &ls,
                       const Point3f &inter_pos,
                       Vec3f &wi,
                       Vec3f *n,
                       float *pdf_solid) const override {
    return beart::Point3f();
  }
  void SampleDirect(const LightSample &ls_pos,
                    const LightSample &ls_dir,
                    Ray &ray,
                    Vec3f &n,
                    float *pdf_solid) const override {

  }
  float DirectPdf(const Point3f &p, const Vec3f &wi) const override {
    return Shape::DirectPdf(p, wi);
  }
  bool Intersect(const Ray &ray) const override;
  bool Intersect(const Ray &ray, SurfaceInterection *inter) const override;
  float SurfaceArea() const override;
  float Volume() const override;
  const AABB &bbox() const override;
 private:
  Vec3f p_0_;
  Vec3f p_1_;
  Vec3f p_2_;

  TriangleMesh *mesh_ = nullptr;
  unsigned int tri_index_ = 0u;
};
}
