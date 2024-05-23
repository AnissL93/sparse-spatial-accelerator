//
// Created by hyl on 15/05/24.
//

#ifndef SIMULATOR_ARCH_CONFIG_H
#define SIMULATOR_ARCH_CONFIG_H

#include "src/config/arch.pb.h"

namespace simu {

arch::Arch parseProto(const std::string& file);

void getRF(arch::Memory* mem, int datasize, int datanum);
void getSRAM(arch::Memory* mem, int datasize, int datanum);

}  // namespace simu

#endif // SIMULATOR_ARCH_CONFIG_H
