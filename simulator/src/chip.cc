//
// Created by hyl on 14/05/24.
//

#include "src/chip.h"
#include "src/tensor.h"
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

arch::Arch Chip::config() const {
  return content->config;
}

void Chip::distribute_to_sram(taco::Tensor<double> &x) {
  int tilex = (int)ceil(x.getDimension(0) / config().x());
  int tiley = (int)ceil(x.getDimension(1) / config().y());
  for (int i = 0; i < content->config.x(); ++i) {
    int stx = i * tilex;
    int sizex = std::min(tilex, x.getDimension(0) - stx);
    for (int j = 0; j < content->config.y(); ++j) {
      int sty = j * tiley;
      int sizey = std::min(tiley, x.getDimension(1) - sty);

      int idx = i * content->config.y() + j;
      PECluster& pec = content->pe.at(idx);
      taco::Tensor<int> subtensor = slice(x, {stx, sty}, {sizex, sizey});
      Memory mem = pec.mem();
    }
  }
}

} // namespace simu
