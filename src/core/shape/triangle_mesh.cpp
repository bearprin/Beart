//
// Created by Bear on 2023/7/12.
//

#include "triangle_mesh.h"
#include "triangle.h"

#include <memory>

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
beart::TriangleMesh::TriangleMesh(std::filesystem::path filename, beart::Transform obj_to_world) : Shape(obj_to_world) {
  Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile(filename.string(), aiProcess_Triangulate);
  if (!scene || !scene->mRootNode) {
    std::cerr << "Failed to load mesh: " << filename << std::endl;   // TODO
  }
  bbox_ = std::make_unique<AABB>();
  // assuming only one mesh now
  const aiMesh *mesh = scene->mMeshes[0];
  // load vertices
  vs_.reserve(mesh->mNumVertices);
  for (size_t i = 0; i < mesh->mNumVertices; ++i) {
    Point3f p{mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};
    p = obj_to_world_ * p;
    bbox_->Union(p);  // update world space bounding box
    vs_.emplace_back(p);
  }
  // load vertex normals
  if (mesh->HasNormals()) {
    vn_.reserve(mesh->mNumVertices);
    for (size_t i = 0; i < mesh->mNumVertices; ++i) {
      vn_.emplace_back(obj_to_world_.TransformNormal({mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z}));
    }
  } else {
    // TODO: compute vertex normals
  }
  // load vertex colors
  if (mesh->HasVertexColors(0)) {
    vc_.reserve(mesh->mNumVertices);
    for (size_t i = 0; i < mesh->mNumVertices; ++i) {
      vc_.emplace_back(mesh->mColors[0][i].r, mesh->mColors[0][i].g, mesh->mColors[0][i].b);
    }
  }
  // load texture coordinates
  if (mesh->HasTextureCoords(0)) {
    uvs_.reserve(mesh->mNumVertices);
    for (size_t i = 0; i < mesh->mNumVertices; ++i) {
      uvs_.emplace_back(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
    }
  }
  // load faces
  Fv_.reserve(mesh->mNumFaces);
  Fn_.reserve(mesh->mNumFaces);
  for (size_t i = 0; i < mesh->mNumFaces; ++i) {
    const aiFace &face = mesh->mFaces[i];
    if (face.mNumIndices != 3) {
      std::cerr << "Only triangle mesh is supported now" << std::endl;   // TODO
    }
    Fv_.emplace_back(face.mIndices[0], face.mIndices[1], face.mIndices[2]);
    // compute face normal (already transform)
    const Point3f &v0 = vs_[face.mIndices[0]];
    const Point3f &v1 = vs_[face.mIndices[0]];
    const Point3f &v2 = vs_[face.mIndices[0]];
    Vec3f e1 = v1 - v0;
    Vec3f e2 = v2 - v0;
    Fn_.emplace_back(Normalize(Cross(e1, e2)));
  }
  for (auto i = 0u; i < Fv_.size(); ++i) {
    std::shared_ptr<Shape> tri = std::make_shared<Triangle>(this, i);
    surface_areas_ += tri->SurfaceArea();
    children_.emplace_back(tri);
  }
  if (Fv_.size() > 100) { // only build bvh for large meshes
    children_bvh_flag_ = true;
  }
}
bool beart::TriangleMesh::Intersect(const beart::Ray &ray) const {
  return std::any_of(children_.begin(), children_.end(), [&ray](const auto &child) {
    return child->Intersect(ray);
  });
}
bool beart::TriangleMesh::Intersect(const beart::Ray &ray, beart::SurfaceInterection *inter) const {
  for (const auto &child : children_) {
    child->Intersect(ray, inter);
  }
  return inter->hit;
}
float beart::TriangleMesh::SurfaceArea() const {
  return surface_areas_;
}
float beart::TriangleMesh::Volume() const {
  // TODO: compute the volume of the mesh
  return 0;
}
const beart::AABB &beart::TriangleMesh::bbox() const {
  if (!bbox_) {
    bbox_ = std::make_unique<AABB>();
    for (const auto &v : vs_) {
      bbox_->Union(v);
    }
  }
  return *bbox_;
}


