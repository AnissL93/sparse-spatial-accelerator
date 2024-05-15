//
// Created by hyl on 14/05/24.
//

#include "src/tensor.h"

namespace simu {

taco::Tensor<double> slice(taco::Tensor<double> x,
                           const std::vector<int> & st,
                           const std::vector<int>& size) {
  assert(st.size() == 2);
  assert(size.size() == 2);
  taco::Format format({taco::Dense, taco::Dense});
  taco::Tensor<double> subtensor(size, format);
  for (int i = 0; i < size[0]; ++i) {
    for (int j = 0; j < size[1]; ++j) {
      subtensor.insert({i, j}, x.at({st[0] + i, st[1] + j}));
    }
  }
  subtensor.setName(x.getName() + "_sub");
  return subtensor;
}

void printMatrix(taco::Tensor<double> &x) {
  int m = x.getDimension(0);
  int n = x.getDimension(1);
  std::cout << "==== " << x.getName() << " [" << m << ", "<< n << "] ===\n";
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < n; ++j) {
      std::cout << x.at({i, j}) << " ";
    }
    std::cout << std::endl;
  }
}

}  // namespace simu
