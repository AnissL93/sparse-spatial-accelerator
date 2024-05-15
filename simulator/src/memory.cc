//
// Created by hyl on 14/05/24.
//

#include "src/memory.h"
#include <memory.h>
#include <cassert>

namespace simu {

// align s to alignment
static size_t align(size_t s, size_t alignment) {
  return (s / alignment) * alignment;
}

struct Memory::Content {
  arch::Memory config;
  uint8_t* data = nullptr;
};

Memory::Memory(arch::Memory mem) {
  auto c = std::make_shared<Content>();
  c->config = mem;
  c->data = new uint8_t[byteSize()];
  memset(c->data, 0, byteSize());
}

size_t Memory::byteSize() const {
  return content->config.size() * content->config.datawidth();
}

Cost Memory::write(const void *src_data, size_t dst_addr, size_t size) {
  assert(dst_addr < byteSize() && "Write address out of range!");
  const auto* src = static_cast<const uint8_t*>(src_data);
  for (size_t i = 0; i < size; ++i) {
    content->data[dst_addr + i] = src[i];
  }

  return writeCost(size);
}

Cost Memory::read(void *dst_data, size_t src_addr, size_t size) const {
  assert(src_addr < byteSize() && "Read address out of range!");
  const auto* dst = static_cast<const uint8_t*>(dst_data);
  for (size_t i = 0; i < size; ++i) {
    content->data[src_addr + i] = dst[i];
  }

  return readCost(size);
}

Cost Memory::writeCost(size_t size) const {
  size_t lat = content->config.latency() +
               content->config.bandwidth() * align(size,
                                                   content->config.datawidth());
  // todo: don't know how to compute
  double energy = 0.;
  return Cost{.latency = lat, .energy = energy};
}

Cost Memory::readCost(size_t size) const {
  size_t lat = content->config.latency() +
               content->config.bandwidth() * align(size,
                                                   content->config.datawidth());
  // todo: don't know how to compute
  double energy = 0.;
  return Cost{.latency = lat, .energy = energy};
}

}  // namespace simu
