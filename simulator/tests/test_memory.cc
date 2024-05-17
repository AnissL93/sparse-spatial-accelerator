//
// Created by hyl on 17/05/24.
//

#include "gtest/gtest.h"
#include "src/memory.h"
#include "src/tensor.h"

using namespace simu;
using namespace taco;

TEST(memory, memory_write) {
  Tensor<int> x({4, 5}, taco::dense);
  fillRandom<int>(x, FillMethod::Dense);
  printMatrix(x);

  arch::Memory mem_config;
  mem_config.set_name("mem");
  mem_config.set_bandwidth(10);
  mem_config.set_datawidth(16);
  mem_config.set_latency(5);
  mem_config.set_size(1024);
  mem_config.set_type(arch::SRAM);
  Memory mem(mem_config);

  mem.writeTensor<int>(x, 0);

  mem.print();
  FAIL();
}
