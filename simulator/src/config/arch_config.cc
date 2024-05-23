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

void getRF(arch::Memory* mem, int datasize, int datanum) {
  mem->set_type(arch::RF);
  mem->set_latency(0);
  mem->set_bandwidth(1);
  mem->set_datawidth(datasize);
  mem->set_name("rf");
  mem->set_size(datanum);
}

void getSRAM(arch::Memory* mem, int datasize, int datanum) {
  mem->set_type(arch::SRAM);
  mem->set_latency(500);
  mem->set_bandwidth(1);
  mem->set_datawidth(datasize);
  mem->set_name("sram");
  mem->set_size(datanum);
}


}  // namespace simu
