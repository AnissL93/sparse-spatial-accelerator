//
// Created by hyl on 21/05/24.
//
#include "gtest/gtest.h"
#include "src/chip.h"

using namespace simu;

arch::Arch makeArch() {
  arch::Arch arch;
  getSRAM(arch.mutable_edge_mem_bottom(), 32, 1024);
  getSRAM(arch.mutable_edge_mem_left(), 32, 1024);
  arch::PE pe_config;
  pe_config.set_vec_lane(4);
  getRF(pe_config.mutable_mem(), 4, 32);

  PE pe(pe_config);

  arch::PECluster cluster;
  cluster.set_pe_num(1);
  getSRAM(cluster.mutable_mem(), 32, 1024);

  *cluster.mutable_pe() = pe_config;

  arch.set_x_num(1);
  arch.set_y_num(4);
  // for 4x4 array
  *arch.mutable_pe_cluster() = cluster;

  return arch;
}

TEST(chip, ChipSparseACC) {
  auto arch = makeArch();
  arch.PrintDebugString();
  Chip chip(arch);
  auto pee = chip.getPECluster(0, 0);
  ASSERT_EQ(chip.getPECluster(0, 1).index().x, 0);
  ASSERT_EQ(chip.getPECluster(0, 1).index().y, 1);

  PE pe = chip.getPECluster(0, 1).pe(0);

  ASSERT_EQ(pe.getRF().byteSize(), 4*32);

  Task<float> task = Task<float>::makeSPMM(16, 16, 16, 0.6, "spmm0");
  task.dump();
  task.setTileSize(1, 4, 16 / arch.y_num());
  chip.execute(task);
  chip.loadA<float>(task.a(), 0, 0, task.getKTileSize());
  chip.print<float>();
  FAIL();
}
