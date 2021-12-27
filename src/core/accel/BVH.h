//
// Created by Bear on 2021/12/27.
//

#ifndef BEART_SRC_CORE_ACCEL_BVH_H_
#define BEART_SRC_CORE_ACCEL_BVH_H_

#include "Accelerator.h"

namespace beart {
class BVH : public Accelerator {
 public:
  struct BVHNode {
    AABB bbox;
    BVHNode *left = nullptr;
    BVHNode *right = nullptr;
    Primitive *obj;
  };
  void Build(const std::vector<std::unique_ptr<const Primitive>> *primitives, const AABB *bbox) override;
  bool IntersectInfo(const Ray &ray, IntersectionInfo *info) const override;
 private:
  void RecursiveGetIntersection(const Ray &ray, IntersectionInfo *info);

};
}

#endif //BEART_SRC_CORE_ACCEL_BVH_H_
