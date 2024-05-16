
#include "src/pe.h"
#include <vector>

namespace simu {

struct PE::Content {
  arch::PE config;
  Memory register_file;
};

PE::PE(arch::PE pe) {
  content = std::make_shared<Content>();
  content->config = pe;
  content->register_file = Memory(pe.mem());
}

arch::PE PE::config() const {
  return content->config;
}

struct PECluster::Content {
  arch::PECluster config;
  std::vector<PE> pes;
  Memory sram;
};

arch::PECluster PECluster::config() const {
  return content->config;
}

PECluster::PECluster(arch::PECluster pe) {
  content = std::make_shared<Content>();
  content->config = pe;
  content->sram = Memory(pe.mem());
  for (int i = 0; i < pe.pes_size(); ++i) {
    content->pes.emplace_back(pe.pes(i));
  }
}

PE PECluster::pe(int idx) {
  assert(idx < content->pes.size() && "pe index out of range!");
  return content->pes[idx];
}

Memory PECluster::mem() {
  return content->sram;
}


}  // namespace simu