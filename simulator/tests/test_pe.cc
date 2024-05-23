#include "gtest/gtest.h"
#include "src/pe.h"
#include "src/tensor.h"

using namespace simu;

TEST(pe, sparsevec) {
  arch::PE pe_config;
  pe_config.set_vec_lane(4);
  pe_config.add_id(0);
  pe_config.add_id(1);
  getRF(pe_config.mutable_mem(), 4, 32);

  PE pe(pe_config);

  arch::PECluster cluster;
  getSRAM(cluster.mutable_mem(), 1024, 32);

  cluster.add_pes();
  *cluster.mutable_pes(0) = pe_config;


  std::cout << cluster.DebugString() << std::endl;

  taco::Tensor<float> v({16}, taco::sparse);
  v.insert({3}, 0.3f);
  v.insert({7}, 0.2f);
  v.pack();

  taco::Tensor<float> m({16, 4}, taco::dense);
  for (int i = 0; i < 16; ++i) {
    for (int j = 0; j < 4; ++j) {
      m(i, j) = 0.1;
    }
  }

  m.pack();
  auto none_zero = pe.sparseVecMatmul(0, v, m);
  std::cout << none_zero << std::endl;

  pe.getRF().print<float>();

  FAIL();
}