//
// Created by Bear on 2023/07/09.
//

#include "event.h"
#include "primitive.h"
beart::Event::Event(const beart::SurfaceInterection &info)
    : n_(Normalize(info.Ns)), b_(Normalize(Cross(n_, Normalize(info.tangent)))),
      t_(Normalize(Cross(b_, n_))), info_(info), bxdfs_(info.primitive->bxdfs()) {
  local_ng_ = WorldToLocal(info_.Ng);
  local_ns_ = WorldToLocal(info_.Ns);
}
beart::Vec3f beart::Event::WorldToLocal(const Vec3f &v) const {
  return {Dot(v, t_), Dot(v, b_), Dot(v, n_)}; // n as z axis
}
beart::Vec3f beart::Event::Local2World(const beart::Vec3f &v) const {
  return t_ * v.x() + b_ * v.y() + n_ * v.z();
}
beart::Spectrum beart::Event::EvaluateBxDF(const beart::Vec3f &wo, const beart::Vec3f &wi) const {
  Vec3f shading_wo = WorldToLocal(wo);
  Vec3f shading_wi = WorldToLocal(wi);
  auto L = Spectrum{0.f, 0.f, 0.f};

  // TODO: assume only one bxdf now
  bxdfs_->operator[](0)->set_geometry_normal(local_ng_);
  bxdfs_->operator[](0)->set_shading_normal(local_ns_);
  L += bxdfs_->operator[](0)->F(shading_wo, shading_wi) * bxdfs_->operator[](0)->eval_weight();
//  for (auto i = 0u; i < bxdfs_->size(); ++i) {
//    bxdfs_->operator[](i)->set_geometry_normal(local_ng);
//    L += bxdfs_->operator[](i)->F(shading_wo, shading_wi);
//  }
  return L;
}
float beart::Event::Pdf(const beart::Vec3f &wo, const beart::Vec3f &wi) const {
  Vec3f shading_wo = WorldToLocal(wo);
  Vec3f shading_wi = WorldToLocal(wi);
  // TODO: assume only one bxdf now
  bxdfs_->operator[](0)->set_geometry_normal(local_ng_);
  bxdfs_->operator[](0)->set_shading_normal(local_ns_);
  return bxdfs_->operator[](0)->Pdf(shading_wo, shading_wi);
}
beart::Spectrum beart::Event::SampleF(const beart::Vec3f &wo,
                                      beart::Vec3f &wi,
                                      const beart::BsdfSample &bs,
                                      float *pdf) const {
  Vec3f shading_wo = WorldToLocal(wo);
  Vec3f shading_wi;
  // TODO: assume only one bxdf now
  auto L = Spectrum{0.f, 0.f, 0.f};
  bxdfs_->operator[](0)->set_geometry_normal(local_ng_);
  bxdfs_->operator[](0)->set_shading_normal(local_ns_);
  L = bxdfs_->operator[](0)->SampleF(shading_wo, shading_wi, bs, pdf);
  wi = Local2World(shading_wi);
  if (*pdf == 0.f) {
    return {0.f};
  }
  return L;
}
