//
// Created by Bear on 2022/4/15.
//

#include "event.h"
#include "primitive.h"
beart::Event::Event(const beart::IntersectionInfo &info)
    : bxdfs_count_(1),
      n_(Normalize(info.Ns)), b_(Normalize(Cross(n_, Normalize(info.tangent)))),
      t_(Normalize(Cross(b_, n_))), info_(info), bxdf_(info.primitive_->material()) {
  bxdf_->set_geometry_normal(World2Local(info.Ng));
  bxdf_->set_shading_normal(World2Local(info.Ns));
}
beart::Vec3f beart::Event::World2Local(const beart::Vec3f &v) const {
  return {Dot(v, t_), Dot(v, b_), Dot(v, n_)};
}
beart::Vec3f beart::Event::Local2World(const beart::Vec3f &v) const {
  return t_ * v.x() + b_ * v.y() + n_ * v.z();
}
beart::Spectrum beart::Event::EvaluateBxDF(const beart::Vec3f &wo, const beart::Vec3f &wi) const {
  auto shading_wo = World2Local(wo);
  auto shading_wi = World2Local(wi);
  auto L = Spectrum{0.f, 0.f, 0.f};
  L += bxdf_->F(shading_wo, shading_wi);
//  for (auto i = 0u; i < bxdfs_count_; ++i) {
//    L += bxdfs_[i]->F(shading_wo, shading_wi);
//  }
  return L;
}
