//
// Created by Bear on 2021/12/27.
//

#pragma once

#include "accelerator.h"
#include "bvh_util.h"

namespace beart {
class BVH : public Accelerator {
 public:
  struct BvhNode {
    unsigned int prim_num = 0;  // the num of primitives
    unsigned int prim_offset = 0; // the offset of the bvh_prim_ptr (1D array)
    AABB bbox;
    std::unique_ptr<BvhNode> left = nullptr;
    std::unique_ptr<BvhNode> right = nullptr;
  };
  void Build(const std::vector<const Primitive *> *primitives, const AABB *bbox) override;
  bool IntersectInfo(const Ray &ray, intersectionInfo *info) const override;
 private:
  std::unique_ptr<BvhNode> root_;
  std::unique_ptr<BvhPrimitive[]> bvh_prims_;
  unsigned max_prim_in_leaf_ = 1;
  unsigned max_depth_ = 16;

  void SplitNode(BvhNode *node, unsigned int start, unsigned int end, unsigned int depth);
  void MakeLeafNode(BvhNode *node, unsigned int start, unsigned int end);

  bool TraverseNode(const BvhNode *node, const Ray &ray, const float t_min, intersectionInfo *info) const;

};
}

