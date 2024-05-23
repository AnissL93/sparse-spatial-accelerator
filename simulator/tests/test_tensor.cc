
#include "taco/tensor.h"
#include "taco.h"
#include <gtest/gtest.h>

#include "src/tensor.h"
#include "tests/utils.h"

using namespace taco;

void fill(Tensor<double>& t, int a, int b) {
  for (int i = 0; i < a; ++i) {
    for (int j = 0; j < b; ++j) {
      double x = i*b+j;
      t.insert({i, j}, x);
    }
  }
}

TEST(sparse, int_spmm) {
  Format dense_2d({Dense, Dense});
  Format csr({Dense, Sparse});

  int m = 3, k = 3, n = 2;

  Tensor<int> A({m,k}, dense_2d);
}

TEST(sparse_matmul, spmm1) {
  Format dense_2d({Dense, Dense});
  Format csr({Dense, Sparse});

  int m = 4, k = 4, n = 4;

  Tensor<double> A({m,k}, dense_2d);
  Tensor<double> B({n,k}, csr);
  Tensor<double> C({m,n}, dense_2d);
  Tensor<double> C_ref({m,n}, dense_2d);

  A.setName("A");
  B.setName("B");
  C.setName("C");
  C_ref.setName("C_ref");

  simu::fillRandom<double>(A, simu::FillMethod::Sparse, 0.5);
  simu::fillRandom<double>(B, simu::FillMethod::Dense, 0.3 );


  IndexVar i, j, kk;
  C(i,j) += A(i,kk) * B(j, kk);

  C.compile();
  C.assemble();
  C.compute();
  C.printAssembleIR(std::cout);
  C.printComputeIR(std::cout);

  simu::matmulRef(A, B, C_ref);

  simu::printMatrix(C_ref);
  simu::printMatrix(C);
  ASSERT_TRUE(equals(C_ref, C));

  auto cc = simu::slice(C, {1, 0}, {2,2});
  simu::printMatrix(C);
  simu::printMatrix(cc);
}