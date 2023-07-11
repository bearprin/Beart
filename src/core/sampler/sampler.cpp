//
// Created by Bear on 2022/4/29.
//

#include "sampler.h"
void beart::Sampler::Request1DArray(const unsigned int size) {
  sample_1d_array_size_.push_back(size);
  sample_1d_.emplace_back(size * sample_count_);
}
void beart::Sampler::Request2DArray(const unsigned int size) {
  sample_2d_array_size_.push_back(size);
  sample_2d_.emplace_back(size * sample_count_);
}
float *beart::Sampler::Next1DArray(const unsigned int size) {
  // from 1d samples get current array
  if (current_1d_array_index_ < sample_1d_.size()) {
    return &(sample_1d_[current_1d_array_index_++][sample_idx * size]);
  }
  return nullptr;
}
beart::Vec2f *beart::Sampler::Next2DArray(const unsigned int size) {
  // from 2d samples get current array
  if (current_2d_array_index_ < sample_2d_.size()) {
    return &(sample_2d_[current_2d_array_index_++][sample_idx * size]);
  }
  return nullptr;
}
void beart::Sampler::Advance() {
  sample_idx++;
  current_1d_array_index_ = 0u;
  current_2d_array_index_ = 0u;
}
