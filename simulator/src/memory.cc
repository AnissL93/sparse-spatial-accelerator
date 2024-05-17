//
// Created by hyl on 14/05/24.
//

#include "src/memory.h"
#include "src/tensor.h"
#include <cassert>
#include <utility>
#include <memory.h>

namespace simu {

// align s to alignment
static size_t align(size_t s, size_t alignment) {
  return (s / alignment) * alignment;
}

struct Memory::Content {
  arch::Memory config;
  void *data = nullptr;

  ~Content() {
    if (data != nullptr) {
      free(data);
      data = nullptr;
    }
  }
};

Memory::Memory(arch::Memory mem) {
  content = std::make_shared<Content>();
  content->config = std::move(mem);
  auto bsize = byteSize();
  content->data = malloc(bsize);
  memset(content->data, 0, bsize);
}

size_t Memory::byteSize() const {
  return content->config.size() * content->config.datawidth();
}

Cost Memory::write(const void *src_data, size_t dst_addr, size_t size) {
  assert(dst_addr < byteSize() && "Write address out of range!");
  const auto *src = static_cast<const uint8_t *>(src_data);
  for (size_t i = 0; i < size; ++i) {
    getPointer<uint8_t>()[dst_addr + i] = src[i];
  }

  return writeCost(size);
}

Cost Memory::read(void *dst_data, size_t src_addr, size_t size) const {
  assert(src_addr < byteSize() && "Read address out of range!");
  const auto *dst = static_cast<const uint8_t *>(dst_data);
  for (size_t i = 0; i < size; ++i) {
    getPointer<uint8_t>()[src_addr + i] = dst[i];
  }

  return readCost(size);
}

Cost Memory::writeCost(size_t size) const {
  size_t lat =
      content->config.latency() +
      content->config.bandwidth() * align(size, content->config.datawidth());
  // todo: don't know how to compute
  double energy = 0.;
  return Cost{.latency = lat, .energy = energy};
}

Cost Memory::readCost(size_t size) const {
  size_t lat =
      content->config.latency() +
      content->config.bandwidth() * align(size, content->config.datawidth());
  // todo: don't know how to compute
  double energy = 0.;
  return Cost{.latency = lat, .energy = energy};
}

template <typename T>
Cost Memory::writeTensor(taco::Tensor<T> tensorBase, size_t addr) {
  T* t = reinterpret_cast<T*>(tensorBase.getTacoTensorT()->vals);
  std::memcpy(getPointer<T>(addr), t, getByteSize<T>(tensorBase));
  return writeCost(getByteSize<T>(tensorBase));
}

template <typename T> Cost Memory::readTensor(taco::Tensor<T> t, size_t addr) {
  size_t size = getByteSize(t);
  auto p = getPointer<T>(addr);
  for (size_t i = 0; i < size; ++i) {
    t(i) = p[i];
  }
  return readCost(size);
}

void Memory::padForAlign(size_t cur_addr, size_t size) {
  int total_byte = align(size, content->config.datawidth());
  int pad_byte = total_byte - size;
  memset((uint8_t *)content->data + cur_addr, 0, pad_byte);
}

template <typename T> T *Memory::getPointer(size_t st_addr_byte) const {
  void *x = (uint8_t *)content->data + st_addr_byte;
  return static_cast<T *>(x);
}

void Memory::print(int line_st, int line_ed) const {
  std::cout << "========= Memory ========= ";
  std::cout << "Size " << byteSize()  << std::endl;
  std::cout << "Bandwidth " << content->config.bandwidth()  << std::endl;
  std::cout << "Datawidth " << content->config.datawidth()  << std::endl;
  std::cout << "Latency " << content->config.latency()  << std::endl;
  auto p = getPointer<int>();
  if (line_ed == -1)
    line_ed = content->config.size();

  for (int i = line_st; i < line_ed; ++i) {
    std::cout << i << ": ";
    for (int j = 0; j < content->config.datawidth() / sizeof(int); ++j) {
      std::cout << *p << "\t";
      p++;
    }
    std::cout << "\n";
  }
}

template Cost Memory::writeTensor<int>(taco::Tensor<int> tensorBase,
                                       size_t addr);
template Cost Memory::writeTensor<float>(taco::Tensor<float> tensorBase,
                                         size_t addr);

template int *Memory::getPointer<int>(size_t st_addr_byte) const;
template float *Memory::getPointer<float>(size_t st_addr_byte) const;

} // namespace simu
