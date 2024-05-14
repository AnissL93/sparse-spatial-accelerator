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
  size_t byte_size = 0;
  size_t datawidth = 0;
  size_t bandwidth = 0;
  size_t latency = 0;
  uint8_t* data = nullptr;
};

Memory::Memory(size_t size, size_t bandwidth, size_t latency, size_t datawidth) {
  auto c = std::make_shared<Content>();
  c->byte_size = size;
  c->bandwidth = bandwidth;
  c->datawidth = datawidth;
  c->latency = latency;
  c->data = new uint8_t[size];
  memset(c->data, 0, size);
}

size_t Memory::byteSize() const {
  return content->byte_size;
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
  size_t lat = content->latency +
               content->bandwidth * align(size, content->datawidth);
  // todo: don't know how to compute
  double energy = 0.;
  return Cost{.latency = lat, .energy = energy};
}

Cost Memory::readCost(size_t size) const {
  size_t lat = content->latency +
               content->bandwidth * align(size, content->datawidth);
  // todo: don't know how to compute
  double energy = 0.;
  return Cost{.latency = lat, .energy = energy};
}

}  // namespace simu
