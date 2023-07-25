//
// Created by Bear on 2023/7/12.
//

#pragma once

#include "shape.h"
#include "accelerator.h"
#include <utility>
#include <vector>
#include <filesystem>
namespace beart {
class TriangleMesh : public Shape {
 public:
  TriangleMesh(fs::path filename, Transform obj_to_world_);
  explicit TriangleMesh(fs::path filename) : TriangleMesh(std::move(filename), Transform{Mat4f{1.0}}) {}
  ~TriangleMesh() override {
    for (auto &tri_pri : triangles_child_prim_) {
      delete tri_pri;
    }
  }
  bool Intersect(const Ray &ray) const override;
  bool Intersect(const Ray &ray, SurfaceInterection *inter) const override;
  float SurfaceArea() const override;
  float Volume() const override;
  const AABB &bbox() const override;
  Point3f SampleDirect(const LightSample &ls,
                       const Point3f &inter_pos,
                       Vec3f &wi,
                       Vec3f *n,
                       float *pdf_solid) const override {
    return beart::Point3f();
  }
  float DirectPdf(const Point3f &p, const Vec3f &wi) const override {
    return Shape::DirectPdf(p, wi);
  }
 private:
  /// Report the approximate size (in bytes) of the mesh
  size_t size() const;

  std::vector<Vec3f> vs_;        ///< Vertex positions
  std::vector<Vec3f> vn_;        ///< Vertex normals
  std::vector<Vec3f> vc_;        ///< Vertex color
  std::vector<Vec2f> uvs_;       ///< Vertex texture coordinates
  std::vector<Vec3i> Fv_;        ///< Vertex indices per face (triangle)
  std::vector<Vec3i> Fn_;        ///< Normal indices per face (triangle)
  std::vector<Vec3i> Ft_;        ///< Texture indices per face (triangle)

  mutable float surface_areas_;
  std::unique_ptr<Accelerator> accelerator_ = nullptr;
  std::vector<std::unique_ptr<Shape>> triangles_child_;
  std::vector<const Primitive *> triangles_child_prim_;
  bool use_bvh_ = false;

  friend class Triangle;
};
}
