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
#include "taco/tensor.h"

namespace simu {

taco::Tensor<double> slice(taco::Tensor<double> x,
                           const std::vector<int> & st,
                           const std::vector<int>& size);

void printMatrix(taco::Tensor<double> &x);
}  // namespace simu

#endif // SIMULATOR_TENSOR_H
