#ifndef SIMULATOR_PE_H
#define SIMULATOR_PE_H

#include "src/config/arch_config.h"
#include "src/memory.h"
#include "src/base_unit.h"
#include <memory>

namespace simu {

struct PEIndex {
  int x;
  int y;
};

class PE : public BaseUnit {
public:
  PE() {}
  PE(arch::PE pe);
  arch::PE config() const;

  template <typename T>
  size_t vec_add(int dst, int r0, int r1);

  template <typename T>
  size_t sparseVecMatmul(int dst, taco::Tensor<T> vec, taco::Tensor<T> mat);

  const Memory&getRF() const;

private:
  struct Content;
  std::shared_ptr<Content> content;
};

class PECluster : public BaseUnit {
public:
  PECluster() {}
  PECluster(const arch::PECluster &pec,
            const PEIndex & index);

  const PE &pe(int idx) const;
  arch::PECluster config() const;
  Memory mem();
  const PEIndex& index() const;
private:
  struct Content;
  std::shared_ptr<Content> content;
};

}  // namespace simu

#endif // SIMULATOR_PE_H
