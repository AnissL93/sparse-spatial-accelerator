//
// Created by hyl on 14/05/24.
//

#ifndef SIMULATOR_TENSOR_H
#define SIMULATOR_TENSOR_H

#include <algorithm>
#include <array>
#include <cstdlib>
#include <numeric>
#include <vector>

class Shape {
public:
  Shape(const std::vector<int64_t> &dims) : dims_(dims) {}

  int rank() const { return dims_.size(); }

  int64_t dim(int i) const { return dims_.at(i); }

  int64_t size() const {
    return std::accumulate(dims_.begin(), dims_.end(), 0);
  }

private:
  std::vector<int64_t> dims_;
};

enum class Device { kGPU, kCPU };

template <typename T> class Tensor {
public:
  Tensor<T> make(size_t byte_size, const Shape &shape,
                 const Device &device = Device::kCPU);
  struct Content;

private:
  Content *content;
};

#endif // SIMULATOR_TENSOR_H
