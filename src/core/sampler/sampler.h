//
// Created by Bear on 2022/4/29.
//

#pragma once

#include "vec.h"
namespace beart {
class Sampler {
 public:
  virtual ~Sampler() = default;
  /// \brief request a space(size) for 1D samples
  /// \param size
  void Request1DArray(const unsigned int size);

  /// \brief request a space(size) for 2D samples
  /// \param size
  void Request2DArray(const unsigned int size);

  /// \brief After requesting space, generate samples to fill the space
  virtual void Generate() = 0;

  /// \brief leverage current space to get new sampler with different seed
  /// \param seed
  /// \return
  virtual std::unique_ptr<Sampler> Clone(unsigned int seed) const = 0;

  virtual float Next1D() = 0;
  virtual Vec2f Next2D() = 0;

  ///
  /// \param size
  /// \return the first address of the array with size
  float *Next1DArray(const unsigned int size);
  Vec2f *Next2DArray(const unsigned int size);

 protected:
  unsigned int sample_count_ = 1;  //
  unsigned int sample_idx = 0;
  std::vector<unsigned int> sample_1d_array_size_;  // save each space size for 1D
  std::vector<unsigned int> sample_2d_array_size_;  // save each space size for 2D
  unsigned int current_1d_array_index_ = 0;  // current 1D array index
  unsigned int current_2d_array_index_ = 0;  // current 2D array index
  std::vector<std::vector<float>> sample_1d_;  // save 1D samples
  std::vector<std::vector<Vec2f>> sample_2d_;  // save 2D samples
};
}
