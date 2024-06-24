//
// Created by hyl on 14/05/24.
//

#include "src/tensor.h"

using namespace taco;
using namespace taco::util;

namespace simu {

template<>
void generateSparseMatrix<float>(taco::TensorBase& tens, double density) {
  std::mt19937 gen(std::random_device{}());
  std::uniform_real_distribution<float> dis_01(0.0, 1.0);
  std::uniform_real_distribution<float> dis_val(-1.0, 1.0);
  unsigned rows = tens.getDimension(0);
  unsigned cols = tens.getDimension(1);
  for (unsigned k = 0; k < rows * cols; ++k) {
    if (dis_01(gen) < density) {
      unsigned i = k / cols;
      unsigned j = k % cols;
      tens.insert({i, j}, dis_val(gen));
    }
  }
  tens.pack();
}

template<>
void generateSparseMatrix<int32_t>(taco::TensorBase& tens, double density) {
  std::mt19937 gen(std::random_device{}());
  std::uniform_int_distribution<int32_t> dis_val(-10, 10);
  std::uniform_real_distribution<float> dis_01(0.0, 1.0);
  unsigned rows = tens.getDimension(0);
  unsigned cols = tens.getDimension(1);
  for (unsigned k = 0; k < rows * cols; ++k) {
    if (dis_01(gen) < density) {
      unsigned i = k / cols;
      unsigned j = k % cols;
      tens.insert({i, j}, dis_val(gen));
    }
  }
  tens.pack();
}


//
} // namespace simu
