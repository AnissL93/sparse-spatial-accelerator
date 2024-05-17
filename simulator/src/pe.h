#ifndef SIMULATOR_PE_H
#define SIMULATOR_PE_H

#include "src/config/arch_config.h"
#include "src/memory.h"
#include "src/base_unit.h"
#include <memory>

namespace simu {

class PE {
public:
  PE() {}
  PE(arch::PE pe);
  arch::PE config() const;

private:
  struct Content;
  std::shared_ptr<Content> content;
};

class PECluster : public BaseUnit<PECluster> {
public:
  PECluster() {}
  PECluster(arch::PECluster pec);

  PE pe(int idx);
  arch::PECluster config() const;
  Memory mem();

private:
  struct Content;
  std::shared_ptr<Content> content;
};

}  // namespace simu

#endif // SIMULATOR_PE_H
