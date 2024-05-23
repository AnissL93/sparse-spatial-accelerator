//
// Created by hyl on 14/05/24.
//

#include "src/memory.h"
#include "src/tensor.h"
#include <bitset>
#include <cassert>
#include <memory.h>
#include <utility>

namespace simu {

// align s to alignment
static size_t align(size_t s, size_t alignment) {
  return (s / alignment) * alignment;
}

struct Memory::Content {
  arch::Memory config;
  uint8_t *data = nullptr;

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
  content->data = (uint8_t *)malloc(bsize);
  memset(content->data, 0, bsize);
}

size_t Memory::byteSize() const {
  return content->config.size() * content->config.datawidth();
}

Cost Memory::writeByte(const void *src_data, size_t dst_addr, size_t size) {
  assert(dst_addr < byteSize() && "Write address out of range!");
  std::memcpy(getPointer<uint8_t>(dst_addr), src_data, size);
  return writeCost(size);
}

Cost Memory::readByte(void *dst_data, size_t src_addr, size_t size) const {
  assert(src_addr < byteSize() && "Read address out of range!");
  std::memcpy(dst_data, getPointer<uint8_t>(src_addr), size);
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
Cost Memory::writeTensor(taco::Tensor<T> &tensorBase, size_t addr) {
  uint8_t *t = tensorBase.getTacoTensorT()->vals;
  size_t size = getByteSize(tensorBase);
  return writeByte(t, addr, size);
}

template <typename T>
Cost Memory::readTensor(taco::Tensor<T> tensorBase, size_t addr) {
  uint8_t *t = tensorBase.getTacoTensorT()->vals;
  size_t size = getByteSize(tensorBase);
  auto c = readByte(t, addr, size);
  tensorBase.pack();
  return c;
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

template <typename T>
void Memory::print(int line_st, int line_ed, bool binary) const {
  std::cout << "========= Memory ========= ";
  std::cout << "is binary: " << binary << std::endl;
  std::cout << "Size " << byteSize() << std::endl;
  std::cout << "Bandwidth " << content->config.bandwidth() << std::endl;
  std::cout << "Datawidth " << content->config.datawidth() << std::endl;
  std::cout << "Latency " << content->config.latency() << std::endl;
  auto p = getPointer<T>();
  uint8_t *p_uint8 = getPointer<uint8_t>();

  if (line_ed == -1)
    line_ed = content->config.size();

  for (int i = line_st; i < line_ed; ++i) {
    std::cout << i << ": ";
    if (binary) {
      for (int j = 0; j < content->config.datawidth(); ++j) {
        auto x = *p_uint8;
        std::cout << std::bitset<8>(x) << " ";
        p_uint8++;
      }
    } else {
      for (int j = 0; j < content->config.datawidth() / sizeof(T); ++j) {
        std::cout << *p << "\t";
        p++;
      }
    }
    std::cout << "\n";
  }
}

template <typename T>
std::vector<T> Memory::profileData(size_t data_addr, size_t data_num) {
  T *p = getPointer<T>(data_addr * sizeof(T));
  std::vector<T> ret(p, p + data_num);
  return ret;
}

template <typename T> T Memory::profileData(size_t data_addr) {
  T *p = getPointer<T>(data_addr * sizeof(T));
  return p[0];
}

const arch::Memory &Memory::config() const { return content->config; }

template void Memory::print<int>(int line_st, int line_ed, bool) const;
template void Memory::print<float>(int line_st, int line_ed, bool) const;
template void Memory::print<uint8_t>(int line_st, int line_ed, bool) const;
template int Memory::profileData<int>(size_t data_addr);
template float Memory::profileData<float>(size_t data_addr);
template std::vector<int> Memory::profileData<int>(size_t data_addr, size_t);
template std::vector<float> Memory::profileData<float>(size_t data_addr,
                                                       size_t);

template Cost Memory::readTensor<int>(taco::Tensor<int> tensorBase,
                                      size_t addr);
template Cost Memory::readTensor<float>(taco::Tensor<float> tensorBase,
                                        size_t addr);
template Cost Memory::writeTensor<int>(taco::Tensor<int> &tensorBase,
                                       size_t addr);
template Cost Memory::writeTensor<float>(taco::Tensor<float> &tensorBase,
                                         size_t addr);

template uint8_t *Memory::getPointer<uint8_t>(size_t st_addr_byte) const;
template int *Memory::getPointer<int>(size_t st_addr_byte) const;
template float *Memory::getPointer<float>(size_t st_addr_byte) const;

} // namespace simu
