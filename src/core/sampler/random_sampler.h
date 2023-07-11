//
// Created by Bear on 2023/07/10.
//
#include <pcg_random.hpp>
#include <random>
namespace beart {
class RandomSampler : public Sampler {
 public:
  RandomSampler() {}
  explicit RandomSampler(unsigned int sample_count) : Sampler(sample_count) {

  }
  std::unique_ptr<Sampler> Clone(unsigned int seed) const override {
    auto *sampler = new RandomSampler(*this);
    sampler->rng_.seed(seed);
    return std::unique_ptr<Sampler>(sampler);
  }
  void Generate() override {
    // generate samples
    for (int j = 0; j < sample_1d_array_size_.size(); ++j) {
      for (int k = 0; k < sample_1d_array_size_[j] * sample_count_; ++k) {
        sample_1d_[j][k] = dist_(rng_);
      }
    }
    for (int j = 0; j < sample_2d_array_size_.size(); ++j) {
      for (int k = 0; k < sample_2d_array_size_[j] * sample_count_; ++k) {
        sample_2d_[j][k] = Vec2f(dist_(rng_), dist_(rng_));
      }
    }
    sample_idx = 0u;
    current_1d_array_index_ = 0u;
    current_2d_array_index_ = 0u;
  }
  float Next1D() const override {
    return dist_(rng_);
  }
  Point2f Next2D() const override {
    return Vec2f{dist_(rng_), dist_(rng_)};
  }
 private:
  mutable std::uniform_real_distribution<float> dist_{0.f, 1.f};
  mutable pcg32 rng_;
};
}
