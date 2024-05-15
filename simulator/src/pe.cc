
#include "src/pe.h"
#include "src/memory.h"
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

struct PECluster::Content {
  arch::PECluster config;
  std::vector<PE> pes;
  Memory sram;
};

PECluster::PECluster(arch::PECluster pe) {
  content = std::make_shared<Content>();
  content->config = pe;
  content->sram = Memory(pe.mem());
  for (int i = 0; i < pe.pes_size(); ++i) {
    content->pes.emplace_back(pe.pes(i));
  }
}


}  // namespace simu