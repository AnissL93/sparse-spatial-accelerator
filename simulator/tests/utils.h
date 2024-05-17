//
// Created by hyl on 17/05/24.
//

#ifndef SIMULATOR_UTILS_H
#define SIMULATOR_UTILS_H

#include "taco/tensor.h"

namespace simu {

using namespace taco;

template<typename T>
void matmulRef(Tensor<T> a, Tensor<T> b, Tensor<T> C_ref,
                Tensor<int> mask = Tensor<int>()) {
  int m = a.getDimension(0);
  int k = a.getDimension(1);
  int n = b.getDimension(0);
  for (int ii = 0; ii < m; ++ii) {
    for (int nn = 0; nn < n; ++nn) {
      for (int jj = 0; jj < k; ++jj) {
        T partial_sum = a.at({ii, jj}) * b.at({nn, jj});
        if (mask.getOrder() == 0) {
          C_ref.insert({ii, nn}, partial_sum);
        } else {
          if (mask.at({ii, nn}) != 0)
            C_ref.insert({ii, nn}, partial_sum);
        }
      }
    }
  }
  C_ref.pack();
}


}  // namespace simu

#endif // SIMULATOR_UTILS_H
