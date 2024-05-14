
#include "src/pe.h"
#include "src/chip.h"
#include <vector>

namespace simu {

struct Chip::Content {
  std::vector<PE> pe_;
  DRAM dram;
};

Chip::Chip(size_t num_pe, size_t dram_size) {
  content = std::make_shared<Content>();
  content->dram = DRAM(dram_size, 32);
}

}  // namespace simu