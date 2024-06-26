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

enum class FillMethod {
  Dense,
  Uniform,
  Random,
  Sparse,
  SlicingH,
  SlicingV,
  FEM,
  HyperSparse,
  Blocked
};

template <typename T> unsigned getNumOfNoneZero(taco::Tensor<T> x) {
  unsigned ret = 0;
  assert(x.getOrder() == 2 && "only support matrix!");
  int m = x.getDimension(0);
  int n = x.getDimension(1);
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < n; ++j) {
      if (x.at({i, j}) != 0) {
        ret++;
      }
    }
  }
  return ret;
}

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
  subtensor.pack();
  subtensor.setName(x.getName() + "_sub");
  return subtensor;
}

template <typename T> void printMatrix(taco::Tensor<T> x) {
  assert(x.getOrder() == 2 && "only support matrix!");
  int m = x.getDimension(0);
  int n = x.getDimension(1);
  std::cout << "==== " << x.getName() << " [" << m << ", " << n
            << "] ===" << getNumOfNoneZero(x) << "/" << m * n << std::endl;

  int width = 10;
  int precision = 4;
  std::cout << std::fixed;                   // Use fixed-point notation
  std::cout << std::setprecision(precision); // Set precision

  for (int i = 0; i < n + 1; ++i) {
    if (i == 0) {
      std::cout << std::setw(width) << " ";
    } else {
      std::cout << std::setw(width) << i;
    }
  }
  std::cout << std::endl;
  std::cout << std::string(width * (n + 1), '-') << std::endl;

  for (int i = 0; i < m; ++i) {
    std::cout << std::setw(width) << i;
    for (int j = 0; j < n; ++j) {
      std::cout << std::setw(width) << x.at({i, j});
    }
    std::cout << std::endl;
  }
}

template <typename>
void generateSparseMatrix(taco::TensorBase &tens, double density);

template <typename T>
void fillRandom(taco::TensorBase &tens, const FillMethod &fill,
                T fillValue = 1., bool binary = false) {
  const std::map<FillMethod, double> fillFactors = {
      {FillMethod::Dense, 1.0},         {FillMethod::Uniform, 1.0},
      {FillMethod::Random, 1.0},        {FillMethod::Sparse, 0.07},
      {FillMethod::HyperSparse, 0.005}, {FillMethod::SlicingH, 0.01},
      {FillMethod::SlicingV, 0.01},     {FillMethod::FEM, 0.0}};

  // Random values
  const float doubleLowerBound = -10e6;
  const float doubleUpperBound = 10e6;
  const int blockDimension = 4;
  const FillMethod blockFillMethod = FillMethod::FEM;

  std::uniform_real_distribution<float> unif(doubleLowerBound,
                                             doubleUpperBound);
  std::default_random_engine re;
  re.seed(std::random_device{}());
  std::vector<int> tensorSize = tens.getDimensions();
  // Random positions
  std::vector<int> pos(tensorSize[0] * tensorSize[1]);
  for (int i = 0; i < static_cast<int>(pos.size()); i++) {
    pos[i] = i;
  }
  std::random_shuffle(pos.begin(), pos.end());
  std::vector<std::vector<int>> positions(tens.getOrder());
  for (int j = 0; j < tens.getOrder(); j++) {
    positions.push_back(std::vector<int>(tensorSize[j]));
    for (int i = 0; i < tensorSize[j]; i++)
      positions[j].push_back(i);
    srand(static_cast<unsigned>(time(NULL)));
    std::random_shuffle(positions[j].begin(), positions[j].end());
  }
  switch (fill) {
  case FillMethod::Dense: {
    for (int i = 0; i < tensorSize[0]; i++) {
      for (int j = 0; j < (fillValue * tensorSize[1]); j++) {
        T val = static_cast<T>(unif(re));
        tens.insert({i, j}, val);
      }
    }
    break;
  }
  case FillMethod::Uniform: {
    for (int i = 0; i < tensorSize[0]; i++) {
      for (int j = 0; j < (fillValue * tensorSize[1]); j++) {
        tens.insert({i, j}, static_cast<T>(1.0));
      }
    }
    break;
  }
  case FillMethod::Sparse:
  case FillMethod::HyperSparse: {
    for (int i = 0; i < (fillValue * tensorSize[0]); i++) {
      for (int j = 0; j < (fillValue * tensorSize[1]); j++) {
        T val = static_cast<T>(unif(re));
        if (binary && val != 0) {
          val = 1;
        }
        tens.insert({positions[0][i], positions[1][j]}, val);
      }
      std::random_shuffle(positions[1].begin(), positions[1].end());
    }
    break;
  }
  case FillMethod::Random: {
    for (int i = 0; i < (fillValue * pos.size()); i++) {
      T val = static_cast<T>(unif(re));
      tens.insert({pos[i] % tensorSize[1], pos[i] / tensorSize[1]}, val);
    }
    break;
  }
  case FillMethod::SlicingH: {
    for (int i = 0; i < (fillValue * tensorSize[0]); i++) {
      for (int j = 0; j < (fillFactors.at(FillMethod::Dense) * tensorSize[1]);
           j++) {
        T val = static_cast<T>(unif(re));
        tens.insert({positions[0][i], positions[1][j]}, val);
      }
    }
    break;
  }
  case FillMethod::SlicingV: {
    for (int j = 0; j < (fillValue * tensorSize[0]); j++) {
      for (int i = 0; i < (fillFactors.at(FillMethod::Dense) * tensorSize[1]);
           i++) {
        T val = static_cast<T>(unif(re));
        tens.insert({positions[0][i], positions[1][j]}, val);
      }
    }
    break;
  }
  case FillMethod::FEM: {
    for (int i = 0; i < tensorSize[0] - 1; i++) {
      tens.insert({i, i}, unif(re));
      double value = unif(re);
      tens.insert({i + 1, i}, value);
      tens.insert({i, i + 1}, value);
      if (i < tensorSize[0] - 3) {
        value = unif(re);
        tens.insert({i + 3, i}, value);
        tens.insert({i, i + 3}, value);
      }
    }
    T val = static_cast<T>(unif(re));
    tens.insert({tensorSize[0] - 1, tensorSize[0] - 1}, val);
    break;
  }
  case FillMethod::Blocked: {
    std::vector<int> dimensions;
    dimensions.push_back(tensorSize[0] / blockDimension);
    dimensions.push_back(tensorSize[1] / blockDimension);
    taco::Tensor<T> BaseTensor(tens.getName(), dimensions, tens.getFormat());
    fillRandom<T>(BaseTensor, blockFillMethod, fillValue);
    for (const auto &elem : BaseTensor) {
      int row = (int)elem.first[0] * blockDimension;
      int col = (int)elem.first[1] * blockDimension;
      T value = elem.second;
      for (int i = 0; i < blockDimension; i++) {
        for (int j = 0; j < blockDimension; j++) {
          T val = static_cast<T>(value / (i + 1));
          tens.insert({row + i, col + j}, val);
        }
      }
    }
    break;
  }
  default: {
    taco_uerror << "FillMethod not available for matrices" << std::endl;
    break;
  }
  }
  tens.pack();
}

template <typename T> size_t getByteSize(taco::Tensor<T> x) {
  int size =
      std::accumulate(x.getDimensions().begin(), x.getDimensions().end(), 1);
  return size * sizeof(T);
}

} // namespace simu

#endif // SIMULATOR_TENSOR_H
