//
// Created by hyl on 14/05/24.
//

#include "src/chip.h"
#include "src/config/arch_config.h"
#include <vector>

namespace simu {

struct Chip::Content {
  arch::Arch config;
  std::vector<PECluster> pe;
  Memory dram;
};

Chip::Chip(arch::Arch c) {
  content = std::make_shared<Content>();
  content->config = c;
  for (int i = 0; i < c.pe_clusters_size(); ++i) {
    content->pe.emplace_back(c.pe_clusters(i));
  }
  content->dram = Memory(c.mem());
}

void Chip::distribute_to_sram(taco::Tensor<double> &x,
                              const std::vector<int> &tile_size) {
  for (int i = 0; i < content->config.x(); ++i) {
    for (int j = 0; j < content->config.y(); ++j) {
      int idx = i * content->config.y() + j;
      PECluster& pec = content->pe.at(idx);
    }
  }
}

} // namespace simu
