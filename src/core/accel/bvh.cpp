//
// Created by Bear on 2021/12/27.
//

#include "bvh.h"
void beart::BVH::Build(const std::vector<const Primitive *> *primitives, const beart::AABB *bbox) {
  Accelerator::Build(primitives, bbox);

  // non primitives, do not construct
  if (primitives_->empty()) {
    return;
  }
  bvh_prims_ = std::make_unique<BvhPrimitive[]>(primitives_->size());

  // init bvh_prims
  for (unsigned int i = 0; i < primitives_->size(); ++i) {
    bvh_prims_[i].set_primitive(primitives_->at(i));
  }

  root_ = std::make_unique<BvhNode>();
  if (primitives_->size() == 1) {
    MakeLeafNode(root_.get(), 0u, primitives_->size());
  } else if (primitives_->size() == 2) {
    root_->left = std::make_unique<BvhNode>();
    root_->left->bbox = bvh_prims_[0].Box();
    MakeLeafNode(root_->left.get(), 0u, 1u);

    root_->right = std::make_unique<BvhNode>();
    root_->right->bbox = bvh_prims_[1].Box();
    MakeLeafNode(root_->right.get(), 1u, 2u);
  } else {
    SplitNode(root_.get(), 0u, primitives_->size(), 1u);
  }

  is_valid_ = true;
}
void beart::BVH::SplitNode(BvhNode *node, const unsigned int start, const unsigned int end, const unsigned int depth) {
  if (start > end) {
    return;
  }
  // union all the bbox primitives
  for (unsigned i = start; i < end; ++i) {
    node->bbox.Union(bvh_prims_[i].Box());
  }
  auto primitives_num = end - start;
  if (primitives_num < max_prim_in_leaf_ || depth == max_depth_) {
    MakeLeafNode(node, start, end);
    return;
  }

  // find the best split plane
  unsigned split_axis;
  float split_pos;
  const float sah = PickBestSplit(bvh_prims_.get(), node->bbox, start, end, &split_axis, &split_pos);
  if (sah >= primitives_num) {
    MakeLeafNode(node, start, end);
    return;
  }
  // get the mid of 1D array
  auto middle = std::partition(bvh_prims_.get() + start,
                               bvh_prims_.get() + end,
                               [&split_axis, &split_pos](const BvhPrimitive &pri) {
                                 return pri.Centroid().data()[split_axis] < split_pos;
                               });
  auto mid = (unsigned int) (middle - bvh_prims_.get());
  if (mid == start || mid == end) {
    MakeLeafNode(node, start, end);
    return;
  }

  node->left = std::make_unique<BvhNode>();
  SplitNode(node->left.get(), start, mid, depth + 1);

  node->right = std::make_unique<BvhNode>();
  SplitNode(node->right.get(), mid, end, depth + 1);
}
void beart::BVH::MakeLeafNode(beart::BVH::BvhNode *node, const unsigned int start, const unsigned int end) {
  node->prim_num = end - start;
  node->prim_offset = start;
}

bool beart::BVH::IntersectInfo(const beart::Ray &ray, beart::IntersectionInfo *info) const {
  if (!is_valid_) {
    return false;
  }
  auto t_min = Intersect(*world_aabb_, ray);
  if (t_min < 0.f) {
    return false;
  }
  return TraverseNode(root_.get(), ray, t_min, info);
}
bool beart::BVH::TraverseNode(const BvhNode *node,
                              const beart::Ray &ray,
                              const float t_min,
                              beart::IntersectionInfo *info) const {
  if (t_min < 0.f) {
    return false;
  }
  if (info && info->happened && info->t_curr < t_min) { // closer than this bbox, return directly
    return true;
  }
  if (node->prim_num != 0) {  // is leaf
    const auto start = node->prim_offset;
    const auto end = node->prim_offset + node->prim_num;
    auto found = false;
    for (auto i = start; i < end; ++i) {
      found |= bvh_prims_[i].primitives_->IntersectInfo(ray, info);
    }
    return found;
  }
  const auto left_ptr = node->left.get();
  const auto right_ptr = node->right.get();

  auto t_min_left = Intersect(left_ptr->bbox, ray);
  auto t_min_right = Intersect(right_ptr->bbox, ray);

  // recursively traverse children
  auto inter = false;
  if (t_min_right > t_min_left) {
    inter |= TraverseNode(left_ptr, ray, t_min_left, info);
    // TODO: Shadow ray
    inter |= TraverseNode(right_ptr, ray, t_min_right, info);
  } else {
    inter |= TraverseNode(right_ptr, ray, t_min_right, info);
    // TODO: Shadow ray
    inter |= TraverseNode(left_ptr, ray, t_min_left, info);
  }
  return inter;
}
