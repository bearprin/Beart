////
//// Created by Bear on 2022/4/29.
////
//
//#pragma once
//
//#include "sampler.h"
//
//#include <pcg_random.hpp>
//#include <random>
//namespace beart {
//class RandomSampler : public Sampler {
// public:
//  explicit RandomSampler() : dist_(0, 1) {
//
//  }
//  std::unique_ptr<Sampler> Clone(unsigned int seed) const override {
//    auto *sampler = new RandomSampler(*this);
//    sampler->rng_.seed(seed);
//    return std::unique_ptr<Sampler>(sampler);
//  }
//  void Generate() override {
//    // generate samples
//    for (int j = 0; j < sample_1d_array_size_.size(); ++j) {
//      for (int k = 0; k < sample_1d_array_size_[j] * sample_count_; ++k) {
//        sample_1d_[j][k] = dist_(rng_);
//      }
//    }
//    for (int j = 0; j < sample_2d_array_size_.size(); ++j) {
//      for (int k = 0; k < sample_2d_array_size_[j] * sample_count_; ++k) {
//        sample_2d_[j][k] = Vec2f(dist_(rng_), dist_(rng_));
//      }
//    }
//  }
//  float Next1D() override {
//    return dist_(rng_);
//  }
//  Vec2f Next2D() override {
//    return Vec2f{dist_(rng_), dist_(rng_)};
//  }
// private:
//  std::uniform_real_distribution<float> dist_;
//  pcg32 rng_;
//};
//}
