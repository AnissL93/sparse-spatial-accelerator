//
// Created by hyl on 14/05/24.
//

#ifndef SIMULATOR_CHIP_H
#define SIMULATOR_CHIP_H

#include "src/config/arch_config.h"
#include "src/memory.h"
#include "src/pe.h"
#include "src/task.h"
#include "taco/tensor.h"

namespace simu {

enum class Direction { E, W, N, S };

class Chip {
public:
  Chip(arch::Arch);

  template <typename T> void distribute_to_sram(const taco::Tensor<T> &x);

  template <typename T> void loadAFromLeft(taco::Tensor<T> &a);

  template <typename T>
  void loadA(const taco::Tensor<T> &a, int m_index, int x_index,
             int k_tile_size);

  template <typename T>
  size_t transfer(const std::pair<int, int> &pe0, const simu::Direction &dir,
                  int dstRID, int srcRID, int data_num);

  const PECluster &getPECluster(int x, int y) const;

  arch::Arch config() const;

  template <typename T> Cost execute(const Task<T> &t);

  const std::vector<PECluster> &getPEArray() const;
  const Memory &getLeftMemory() const;
  const Memory &getBottomMemory() const;

  template <typename T> void print(bool sram = true, bool reg = true) const;

private:
  struct Content;
  std::shared_ptr<Content> content;
};

} // namespace simu

#endif // SIMULATOR_CHIP_H
