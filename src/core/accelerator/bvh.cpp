//
// Created by Bear on 2023/07/20.
//

#include "bvh.h"
#include "aabb.h"
void beart::BVH::Build(const std::vector<const Primitive *> *primitives, const beart::AABB *bbox) {
  Accelerator::Build(primitives, bbox);

  // non primitives, do not construct
  if (primitives_->empty()) {
    return;
  }
  unsigned int bvh_prim_size{};
  bvh_prim_size += primitives_->size();
  bvh_prims_ = std::make_unique<BvhPrimitive[]>(bvh_prim_size);

  // init bvh_prims
  unsigned int bvh_prims_index{};
  for (auto primitive : *primitives_) {
    bvh_prims_[bvh_prims_index++].set_primitive(primitive);
  }

  root_ = std::make_unique<BvhNode>();
  if (bvh_prim_size == 1) {
    MakeLeafNode(root_.get(), 0u, primitives_->size());
  } else if (bvh_prim_size == 2) {
    root_->left = std::make_unique<BvhNode>();
    root_->left->bbox = bvh_prims_[0].Box();
    MakeLeafNode(root_->left.get(), 0u, 1u);

    root_->right = std::make_unique<BvhNode>();
    root_->right->bbox = bvh_prims_[1].Box();
    MakeLeafNode(root_->right.get(), 1u, 2u);
  } else {
    SplitNode(root_.get(), 0u, bvh_prim_size, 0u);
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
  if (primitives_num < max_prim_in_leaf_ || depth >= max_depth_) {
    MakeLeafNode(node, start, end);
    return;
  }

  // find the best split plane
  unsigned split_axis{};
  float split_pos{};
  const float sah = PickBestSplit(bvh_prims_.get(), node->bbox, start, end, &split_axis, &split_pos);
  if (sah >= static_cast<float>(primitives_num)) {
    MakeLeafNode(node, start, end);
    return;
  }
  // get the mid of 1D array
  auto middle = std::partition(bvh_prims_.get() + start, bvh_prims_.get() + end,
                               [split_axis, split_pos](const BvhPrimitive &prim) {
                                 return prim.Centroid()[split_axis] < split_pos;
                               });
  auto mid = std::distance(bvh_prims_.get(), middle);
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

bool beart::BVH::Intersect(const beart::Ray &ray, beart::SurfaceInterection *info) const {
  if (!is_valid_) {
    return false;
  }
  auto t_min = AABB::Intersect(*world_aabb_, ray);
  if (t_min < 0.f) {
    return false;
  }
  return TraverseNode(root_.get(), ray, t_min, info);
}
bool beart::BVH::TraverseNode(const BvhNode *node,
                              const beart::Ray &ray,
                              const float t_min,
                              beart::SurfaceInterection *info) const {
  if (t_min < 0.f) {
    return false;
  }
  if (info && info->t_curr < t_min) { // closer than this bbox, return directly
    return true;
  }
  if (node->prim_num != 0) {  // is leaf
    const auto start = node->prim_offset;
    const auto end = node->prim_offset + node->prim_num;
    auto found = false;
    auto temp_r = ray;
    temp_r.t_max_ = info->t_curr < temp_r.t_max_ ? info->t_curr : temp_r.t_max_;
    SurfaceInterection temp_info = *info;
    for (auto i = start; i < end; ++i) {
      if (bvh_prims_[i].primitive_->Intersect(temp_r, &temp_info)) {
        found = true;
        temp_r.t_max_ = temp_info.t_curr;
        if (!ray.is_primary_ray_) return true; // shadow ray, return directly
      }
    }
    if (temp_info.t_curr < info->t_curr) {
      *info = temp_info;
    }
    return found;
  }
  const auto left_ptr = node->left.get();
  const auto right_ptr = node->right.get();

  auto t_min_left = AABB::Intersect(left_ptr->bbox, ray);
  auto t_min_right = AABB::Intersect(right_ptr->bbox, ray);

  // recursively traverse children
  bool inter = false;
  if (t_min_right > t_min_left) {
    inter |= TraverseNode(left_ptr, ray, t_min_left, info);
    if (inter && !ray.is_primary_ray_) return true;
    inter |= TraverseNode(right_ptr, ray, t_min_right, info);
  } else {
    inter |= TraverseNode(right_ptr, ray, t_min_right, info);
    if (inter && !ray.is_primary_ray_) return true;
    inter |= TraverseNode(left_ptr, ray, t_min_left, info);
  }
  return inter;
}
bool beart::BVH::IsOccupied(const beart::Ray &ray) const {
  SurfaceInterection info;
  return Intersect(ray, &info);
}
