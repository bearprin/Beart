//
// Created by Bear on 2023/07/09.
//

#include "event.h"
#include "primitive.h"
beart::Event::Event(const beart::SurfaceInterection &info)
    : n_(Normalize(info.Ns)), b_(Normalize(Cross(n_, Normalize(info.tangent)))),
      t_(Normalize(Cross(b_, n_))), info_(info), bxdfs_(info.primitive->bxdfs()) {
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
  Vec3f local_ng = WorldToLocal(info_.Ng);
  Vec3f local_ns = WorldToLocal(info_.Ns);
  for (auto i = 0u; i < bxdfs_->size(); ++i) {
    bxdfs_->operator[](i)->set_geometry_normal(local_ng);
    L += bxdfs_->operator[](i)->F(shading_wo, shading_wi);
  }
  return L;
}
