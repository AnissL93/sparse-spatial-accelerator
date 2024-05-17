//
// Created by hyl on 14/05/24.
//

#ifndef SIMULATOR_MEMORY_H
#define SIMULATOR_MEMORY_H

#include "src/config/arch_config.h"
#include <iostream>
#include <memory>
#include <optional>
#include "taco/tensor.h"
#include "src/base_unit.h"

namespace simu {


struct Cost {
  size_t latency;
  double energy;
};

/*
 * Base class for all memory storage, including DRAM, SRAM and register files
 */
class Memory : public BaseUnit<Memory> {
public:
  Memory() {};
  Memory(arch::Memory mem);
  ~Memory() = default;
  // Get cost for reading data of size
  Cost readCost(size_t size) const;
  // Get cost for writing data of size
  Cost writeCost(size_t size) const;
  // Read data from this memory buffer to dst_data.
  Cost read(void* dst_data, size_t src_addr, size_t size) const;
  // Write data in src_data to this memory buffer.
  Cost write(const void* src_data, size_t dst_addr, size_t size) ;
  // Write the entire tensor to memory
  template <typename T>
  Cost writeTensor(taco::Tensor<T> tensorBase, size_t addr);
  // Read the entire tensor from memory
  template <typename T>
  Cost readTensor(taco::Tensor<T> t, size_t addr);
  size_t byteSize() const;

  void print(int lines = 0, int line_ed = -1) const;

private:
  struct Content;
  std::shared_ptr<Content> content;
  template <typename T> T* getPointer(size_t st_addr = 0) const;
  void padForAlign(size_t cur_addr, size_t size);
};

}  // namespace simu
#endif // SIMULATOR_MEMORY_H
