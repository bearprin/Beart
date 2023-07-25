//
// Created by Bear on 2023/07/20.
//

#pragma once

#include "shape.h"
#include "common.h"

namespace beart {

struct BvhPrimitive {
  const Primitive *primitive_ = nullptr;

  void set_primitive(const Primitive *prim) {
    primitive_ = prim;
  }
  [[nodiscard]] const AABB &Box() const {
    return primitive_->bbox();
  }
  [[nodiscard]] Vec3f Centroid() const {
    return primitive_->shape()->bbox().Centroid();
  }

};
struct BinInfo {
  unsigned int count = 0;
  AABB bin_box;
};

inline float sah_eval(const unsigned int left_prim_num,
                      const unsigned int right_prim_num,
                      const AABB &l_box,
                      const AABB &r_box,
                      const AABB &box) {
  return (left_prim_num * l_box.HalfSurfaceArea() + right_prim_num * r_box.HalfSurfaceArea()) / box.HalfSurfaceArea();
}
inline float PickBestSplit(
    const BvhPrimitive *primitive,
    const AABB &node_bbox,
    const unsigned int start,
    const unsigned int end,
    unsigned int *axis_id,
    float *split_pos) {

  assert(start < end);

  static constexpr unsigned int kBvhBinsNum = 32;
//  static constexpr float kInvBvhBinsNum = 1.f / static_cast<float>(kBvhBinsNum);
  // the bounds for all centroids
  AABB inner;
  for (unsigned int i = start; i < end; ++i) {
    inner.Union(primitive[i].Centroid());
  }
  auto min_sah = kMaxFloat;
  auto primitives_num = end - start;
  // using the max axis extents as axis not test all 3 axis
  *axis_id = inner.MaxAxisExtents();

  // distributed prim to bins
  BinInfo bin_info[kBvhBinsNum];
  auto k0 = inner[0].data()[*axis_id];
  auto delta = inner.Delta(*axis_id);
  if (delta == 0.f) {
    return kMaxFloat;
  }
  auto k1 = kBvhBinsNum * (1.f / delta);  // the relation unit length about bins
  for (auto i = start; i < end; ++i) {
    unsigned int index = k1 * (primitive[i].Centroid()[*axis_id] - k0);  // find the bins of primitive i
    index = std::min(index, kBvhBinsNum - 1);
    bin_info[index].count += 1;
    bin_info[index].bin_box.Union(primitive[i].Box());  // update bin box
  }
  // init rbox (sweep from right) to avoid O(n^2)
  AABB rbox[kBvhBinsNum - 1];
  rbox[kBvhBinsNum - 2].Union(bin_info[kBvhBinsNum - 1].bin_box);
  for (int i = kBvhBinsNum - 3; i >= 0; --i) {
    rbox[i] = Union(rbox[i + 1], bin_info[i + 1].bin_box);
  }
  // sweep from left
  auto left = bin_info[0].count;
  auto lbox = bin_info[0].bin_box;
  // TODO: Parallel
  std::vector<float> cost;
  for (auto i = 0; i < kBvhBinsNum - 1; ++i) {
    auto sah_value = sah_eval(left, primitives_num - left, lbox, rbox[i], node_bbox);
    left += bin_info[i + 1].count;
    lbox.Union(bin_info[i + 1].bin_box);
    cost.push_back(sah_value);
  }
  auto min_cost = std::min_element(cost.begin(), cost.end());
  auto min_cost_index = std::distance(cost.begin(), min_cost);
  min_sah = *min_cost;
  *split_pos = k0 + delta / kBvhBinsNum * (min_cost_index + 1);
  return min_sah;
}
}