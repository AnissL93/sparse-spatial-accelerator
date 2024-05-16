
#include "taco/tensor.h"
#include "taco.h"
#include <gtest/gtest.h>

#include "src/tensor.h"

using namespace taco;

void fill(Tensor<double>& t, int a, int b) {
  for (int i = 0; i < a; ++i) {
    for (int j = 0; j < b; ++j) {
      double x = i*b+j;
      t.insert({i, j}, x);
    }
  }
}

TEST(sparse_matmul, spmm1) {
  Format dense_2d({Dense, Dense});
  Format csr({Dense, Sparse});

  int m = 3, k = 3, n = 2;

  Tensor<double> A({m,k}, dense_2d);
  Tensor<double> B({n,k}, csr);
  Tensor<double> C({m,n}, dense_2d);
  Tensor<double> C_ref({m,n}, dense_2d);

  A.setName("A");
  B.setName("B");
  C.setName("C");
  C_ref.setName("C_ref");
//
//  fill(A, m, k);
//  fill(B,n, k);

//  simu::fillRandom<double>(A, util::FillMethod::Dense );
//  simu::fillRandom<double>(B, util::FillMethod::Dense, 0.3 );


  IndexVar i, j, kk;
  C(i,j) += A(i,kk) * B(j, kk);

  C.compile();
  C.assemble();
  C.compute();
  C.printAssembleIR(std::cout);
  C.printComputeIR(std::cout);

  for (int ii = 0; ii < m; ++ii) {
    for (int nn = 0; nn < n; ++nn) {
      for (int jj = 0; jj < k; ++jj) {
        double partial_sum = A.at({ii, jj}) * B.at({nn, jj});
        C_ref.insert({ii, nn}, partial_sum);
      }
    }
  }
  ASSERT_TRUE(equals(C_ref, C));

  auto cc = simu::slice(C, {1, 0}, {2,2});
  simu::printMatrix(C);
  simu::printMatrix(cc);

  FAIL();
}
//
//int main() {
//  Format dense_2d({Dense, Dense});
//  Tensor<float> A({3,4}, dense_2d);
//  A.setName("denseM");
//  for (int i = 0; i < 3; ++i) {
//    for (int j = 0; j < 4; ++j) {
//      float x = i / 10. + j;
//      A.insert({i, j}, x);
//    }
//  }
//
//  float xx = A.at({0, 3});
//  std::cout << xx << std::endl;
//
////  // Create formats
////  Format csr({Dense,Sparse});
////  Format csf({Sparse,Sparse,Sparse});
////  Format  sv({Sparse});
////
////  // Create tensors
////  Tensor<double> A({2,3},   csr);
////  Tensor<double> B({2,3,4}, csf);
////  Tensor<double> c({4},     sv);
////
////  A.setName("A_mat");
////
////  // Insert data into B and c
////  B.insert({0,0,0}, 1.0);
////  B.insert({1,2,0}, 2.0);
////  B.insert({1,2,1}, 3.0);
////  c.insert({0}, 4.0);
////  c.insert({1}, 5.0);
////
////  // Pack inserted data as described by the formats
////  B.pack();
////  c.pack();
////
////  // Form a tensor-vector multiplication expression
////  IndexVar i, j, k;
////  A(i,j) = B(i,j,k) * c(k);
////
////  // Compile the expression
////  A.compile();
////
////  // Assemble A's indices and numerically compute the result
////  A.assemble();
////  A.compute();
////
////
////  std::cout << B << std::endl;
////  std::cout << c << std::endl;
//
//  std::cout << A << std::endl;
//
//
//
//  return 0;
//}