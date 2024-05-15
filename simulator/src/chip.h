//
// Created by hyl on 14/05/24.
//

#ifndef SIMULATOR_CHIP_H
#define SIMULATOR_CHIP_H

#include "src/config/arch_config.h"
#include "src/memory.h"
#include "src/pe.h"
#include "taco/tensor.h"

namespace simu {

class Chip {
public:
  Chip(arch::Arch);

  void distribute_to_sram(taco::Tensor<double> &x,
                          const std::vector<int> &tile_size);

private:
  struct Content;
  std::shared_ptr<Content> content;
};

}  // namespace simu

#endif // SIMULATOR_CHIP_H
