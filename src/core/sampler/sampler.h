//
// Created by Bear on 2022/4/29.
//

#pragma once

#include "vec.h"
#include "json_serializable.h"

#include <vector>
namespace beart {
class Sampler {
 public:
  Sampler() = default;
  explicit Sampler(unsigned int sample_count) : sample_count_(sample_count) {}
  Sampler(const json &j) : sample_count_(j.value("samples", 1)) {}
  virtual ~Sampler() = default;
  /// \brief request a space(size) for 1D samples
  /// \param size
  void Request1DArray(unsigned int size);

  /// \brief request a space(size) for 2D samples
  /// \param size
  void Request2DArray(unsigned int size);

  /// \brief After requesting space, generate samples to fill the space
  virtual void Generate() = 0;

  /// \brief leverage current space to get new sampler with different seed
  /// \param seed
  /// \return
  virtual std::unique_ptr<Sampler> Clone(unsigned int seed) const = 0;

  /// Step to next sample
  virtual void Advance();

  virtual float Next1D() const = 0;
  virtual Point2f Next2D() const = 0;

  unsigned int sample_count() const {
    return sample_count_;
  }

  ///
  /// \param size
  /// \return the first address of the array with size
  float *Next1DArray(unsigned int size);
  Point2f *Next2DArray(unsigned int size);

 protected:
  unsigned int sample_count_ = 1;  // default sample count
  unsigned int sample_idx = 0;
  std::vector<size_t> sample_1d_array_size_;  // save each space size for 1D
  std::vector<size_t> sample_2d_array_size_;  // save each space size for 2D
  unsigned int current_1d_array_index_ = 0;  // current 1D array index
  unsigned int current_2d_array_index_ = 0;  // current 2D array index
  std::vector<std::vector<float>> sample_1d_;  // save 1D samples
  std::vector<std::vector<Vec2f>> sample_2d_;  // save 2D samples
};
}
