
#include "src/pe.h"
#include <vector>

namespace simu {

struct PE::Content {
  arch::PE config;
  Memory register_file;
};

PE::PE(arch::PE pe) {
  content = std::make_shared<Content>();
  content->config = pe;
  content->register_file = Memory(pe.mem());
}

arch::PE PE::config() const { return content->config; }

const Memory &PE::getRF() const {
  return content->register_file;
}

// scalar mult vector
template <typename T> void SMV(T *dst, T s, T *vec, int len) {
  for (int i = 0; i < len; ++i) {
    dst[i] = s * vec[i];
  }
}

template <typename T>
size_t PE::sparseVecMatmul(int dst, taco::Tensor<T> vec, taco::Tensor<T> mat) {
  assert(vec.getOrder() == 1);
  assert(vec.getDimensions()[0] == mat.getDimension(0));

  auto none_zero_num = vec.getStorage().getValues().getSize();

  vec.setName("vec");
  mat.setName("mat");

  taco::Tensor<T> partial_sum({config().vec_lane()}, taco::dense);
  taco::IndexVar i, j;
  partial_sum(j) += vec(i) * mat(i, j);
  partial_sum.compile();
  partial_sum.assemble();
  partial_sum.compute();

  partial_sum.setName("partial");
  std::cout << partial_sum << std::endl;

  // add partial to dst register
  auto *p = content->register_file.getPointer<T>(dst * sizeof(T));
  for (int k = 0; k < config().vec_lane(); ++k) {
    p[k] += partial_sum(k);
  }

  return none_zero_num;
}

struct PECluster::Content {
  arch::PECluster config;
  std::vector<PE> pes;
  Memory sram;
  PEIndex index;
};

arch::PECluster PECluster::config() const { return content->config; }

PECluster::PECluster(const arch::PECluster &pe,
                     const PEIndex& index) {
  content = std::make_shared<Content>();
  content->config = pe;
  content->sram = Memory(pe.mem());
  for (int i = 0; i < pe.pe_num(); ++i) {
    content->pes.emplace_back(pe.pe());
  }
  content->index = index;
}

const PE &PECluster::pe(int idx) const {
  assert(idx < content->pes.size() && "pe index out of range!");
  return content->pes[idx];
}

const PEIndex& PECluster::index() const {
  return content->index;
}

Memory PECluster::mem() { return content->sram; }

template size_t PE::sparseVecMatmul<int>(int dst, taco::Tensor<int> vec,
                                         taco::Tensor<int> mat);
template size_t PE::sparseVecMatmul<float>(int dst, taco::Tensor<float> vec,
                                         taco::Tensor<float> mat);

} // namespace simu