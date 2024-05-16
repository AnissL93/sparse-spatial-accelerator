//
// Created by hyl on 14/05/24.
//

#ifndef SIMULATOR_TENSOR_H
#define SIMULATOR_TENSOR_H

#include "taco/tensor.h"
#include "taco/util/fill.h"
#include <algorithm>
#include <array>
#include <cstdlib>
#include <numeric>
#include <vector>

namespace simu {

using taco::util::FillMethod;

template <typename T>
taco::Tensor<T> slice(taco::Tensor<T> x, const std::vector<int> &st,
                      const std::vector<int> &size) {
  assert(st.size() == 2);
  assert(size.size() == 2);
  taco::Format format({taco::Dense, taco::Dense});
  taco::Tensor<T> subtensor(size, format);
  for (int i = 0; i < size[0]; ++i) {
    for (int j = 0; j < size[1]; ++j) {
      subtensor.insert({i, j}, x.at({st[0] + i, st[1] + j}));
    }
  }
  subtensor.setName(x.getName() + "_sub");
  return subtensor;
}

template <typename T> void printMatrix(taco::Tensor<T> &x) {
  assert(x.getOrder() == 2 && "only support matrix!");
  int m = x.getDimension(0);
  int n = x.getDimension(1);
  std::cout << "==== " << x.getName() << " [" << m << ", " << n << "] ===\n";
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < n; ++j) {
      std::cout << x.at({i, j}) << " ";
    }
    std::cout << std::endl;
  }
}

//template <typename T>
//void fillRandom(taco::TensorBase &tens, const taco::util::FillMethod &fill,
//                double fillValue = 1.);

} // namespace simu

#endif // SIMULATOR_TENSOR_H
