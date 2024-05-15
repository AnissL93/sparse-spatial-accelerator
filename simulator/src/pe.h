#ifndef SIMULATOR_PE_H
#define SIMULATOR_PE_H

#include "src/config/arch_config.h"
#include <memory>

namespace simu {

class PE {
public:
  PE() {}
  PE(arch::PE pe);

private:
  struct Content;
  std::shared_ptr<Content> content;
};

class PECluster {
public:
  PECluster() {}
  PECluster(arch::PECluster pec);

private:
  struct Content;
  std::shared_ptr<Content> content;
};

}  // namespace simu

#endif // SIMULATOR_PE_H
