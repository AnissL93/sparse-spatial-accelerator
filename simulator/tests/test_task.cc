//
// Created by hyl on 15/05/24.
//

#include "src/task.h"
#include "src/tensor.h"
#include "tests/utils.h"
#include "gtest/gtest.h"

using namespace simu;

TEST(task, spmm) {
  Task<double> spmm = Task<double>::makeSPMM(10, 8, 4, 0.2, "spmm0");
  spmm.compute();
  Tensor<double> ref(spmm.c().getDimensions());
  matmulRef(spmm.a(), spmm.b(), ref);
  ASSERT_TRUE(equals(spmm.c(), ref));
}

TEST(task, sddm) {
  {
    Task<double> sddm = Task<double>::makeSDDM(10, 8, 4, 0.5, "spmm0");
    sddm.compute();
    Tensor<double> ref("ref", sddm.c().getDimensions());
    matmulRef(sddm.a(), sddm.b(), ref, sddm.mask());
    ASSERT_TRUE(equals(sddm.c(), ref));
  }

  {
    Task<int> sddm = Task<int>::makeSDDM(10, 8, 4, 0.5, "spmm0");
    sddm.compute();
    Tensor<int> ref("ref", sddm.c().getDimensions());
    matmulRef<int>(sddm.a(), sddm.b(), ref, sddm.mask());
    ASSERT_TRUE(equals(sddm.c(), ref));

    printMatrix(ref);
    int total_size = 0;
    for (auto iter = ref.beginTyped<int>(); iter != ref.endTyped<int>();
         ++iter) {
      std::cout << iter->second << "\n";
      total_size++;
    }
    std::cout << total_size << std::endl;
    FAIL();
  }
}
