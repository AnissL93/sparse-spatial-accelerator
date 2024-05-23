//
// Created by hyl on 17/05/24.
//

#include "gtest/gtest.h"
#include "src/memory.h"
#include "src/tensor.h"
#include <bitset>

using namespace simu;
using namespace taco;

TEST(memory, register_file) {
  Tensor<int> x({20, 1}, taco::dense);
  for (int i = 0; i < 20; ++i) {
    x.insert({i, 0}, i);
  }
  x.pack();
  //  fillRandom<int>(x, FillMethod::Dense);
  printMatrix(x);

  std::cout << std::bitset<8>(10) << std::endl;

  arch::Memory mem_config;
  mem_config.set_name("mem");
  mem_config.set_bandwidth(10);
  mem_config.set_datawidth(16);
  mem_config.set_latency(5);
  mem_config.set_size(1024);
  mem_config.set_type(arch::SRAM);
  Memory mem(mem_config);

  mem.writeTensor<int>(x, 0);
  mem.writeTensor<int>(x, 4*20 + 128);
  //  mem.writeTensor<int>(x, 20*4 + 10);

  std::cout << "finish write\n";

  Tensor<int> y({20, 1}, taco::dense);
  y.pack();
  printMatrix(y);
  mem.readTensor(y, 8);
  printMatrix(y);
  mem.print<int>(0, 50, false);


  Tensor<int> sp({10}, sparse);
  sp.insert({0}, 10);
  sp.insert({5}, 10);
  sp.insert({7}, 10);
  sp.pack();
  std::cout << sp << std::endl;
  std::cout << sp.getStorage().getValues() << std::endl;

  for(auto iter = sp.begin(); iter != sp.end(); ++iter) {
    std::cout << iter->first << std::endl;
    std::cout << iter->second << "\n";
  }

  auto yy = y.at({0, 1});

  FAIL();
}

TEST(memory, memory_write) {
  Tensor<int> x({20, 1}, taco::dense);
  for (int i = 0; i < 20; ++i) {
    x.insert({i, 0}, i);
  }
  x.pack();
//  fillRandom<int>(x, FillMethod::Dense);
  printMatrix(x);

  std::cout << std::bitset<8>(10) << std::endl;

  arch::Memory mem_config;
  mem_config.set_name("mem");
  mem_config.set_bandwidth(10);
  mem_config.set_datawidth(16);
  mem_config.set_latency(5);
  mem_config.set_size(1024);
  mem_config.set_type(arch::SRAM);
  Memory mem(mem_config);

  mem.writeTensor<int>(x, 0);
  mem.writeTensor<int>(x, 4*20 + 128);
//  mem.writeTensor<int>(x, 20*4 + 10);

  std::cout << "finish write\n";

  Tensor<int> y({20, 1}, taco::dense);
  y.pack();
  printMatrix(y);
  mem.readTensor(y, 8);
  printMatrix(y);
  mem.print<int>(0, 50, false);


  Tensor<int> sp({10}, sparse);
  sp.insert({0}, 10);
  sp.insert({5}, 10);
  sp.insert({7}, 10);
  sp.pack();
  std::cout << sp << std::endl;
  std::cout << sp.getStorage().getValues() << std::endl;

  for(auto iter = sp.begin(); iter != sp.end(); ++iter) {
    std::cout << iter->first << std::endl;
    std::cout << iter->second << "\n";
  }

  auto yy = y.at({0, 1});

  FAIL();
}
