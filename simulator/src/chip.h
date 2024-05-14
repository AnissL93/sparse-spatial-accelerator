//
// Created by hyl on 14/05/24.
//

#ifndef SIMULATOR_CHIP_H
#define SIMULATOR_CHIP_H

#include "src/pe.h"
#include "src/memory.h"

namespace simu {

class Chip {
public:

  Chip(size_t num_pe, size_t dram = -1 /*-1 means infinit*/);

private:
  struct Content;
  std::shared_ptr<Content> content;
};

}  // namespace simu

#endif // SIMULATOR_CHIP_H
