//
// Created by hyl on 15/05/24.
//

#include "src/config/arch_config.h"
#include <fstream>

namespace simu {

arch::Arch parseProto(const std::string& file) {
  arch::Arch arch;
  std::ifstream input(file.c_str());
  if (!arch.ParseFromIstream(&input)) {
    std::cerr << "Failed to parse Person message." << std::endl;
    exit(-1);
  }
  return arch;
}

}  // namespace simu
