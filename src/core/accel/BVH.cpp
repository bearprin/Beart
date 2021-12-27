//
// Created by Bear on 2021/12/27.
//

#include "BVH.h"
void beart::BVH::Build(const std::vector<std::unique_ptr<const Primitive>> *primitives, const beart::AABB *bbox) {
  Accelerator::Build(primitives, bbox);
}
bool beart::BVH::IntersectInfo(const beart::Ray &ray, beart::IntersectionInfo *info) const {
  return false;
}
