//
// Created by hyl on 14/05/24.
//

#ifndef SIMULATOR_MEMORY_H
#define SIMULATOR_MEMORY_H

#include <iostream>
#include <memory>
#include <optional>

namespace simu {


struct Cost {
  size_t latency;
  double energy;
};

/*
 * Base class for all memory storage, including DRAM, SRAM and register files
 */
class Memory {
public:
  Memory() {};
  Memory(size_t size, size_t bandwidth, size_t latency, size_t datawidth);
  ~Memory() = default;
  // Get cost for reading data of size
  Cost readCost(size_t size) const;
  // Get cost for writing data of size
  Cost writeCost(size_t size) const;
  // Read data from this memory buffer to dst_data.
  Cost read(void* dst_data, size_t src_addr, size_t size) const;
  // Write data in src_data to this memory buffer.
  Cost write(const void* src_data, size_t dst_addr, size_t size) ;

  size_t byteSize() const;

private:
  struct Content;
  std::shared_ptr<Content> content;
};

class RegisterFile : public Memory {
public:
  RegisterFile() : Memory() {}
  RegisterFile(size_t num, size_t width) : Memory(num * width, 0, 0, width) {}
};

class SRAM : public Memory {
public:
  SRAM() : Memory() {}
  SRAM(size_t line_num, size_t width) : Memory(line_num * width, 0, 0, width) {}
};

class DRAM : public Memory {
public:
  DRAM() : Memory() {}
  DRAM(size_t line_num, size_t width) : Memory(line_num * width, 0, 0, width) {}
};

}  // namespace simu
#endif // SIMULATOR_MEMORY_H
