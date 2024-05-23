//
// Created by hyl on 14/05/24.
//

#include "src/chip.h"
#include "src/config/arch_config.h"
#include "src/tensor.h"
#include <vector>

namespace simu {

struct Chip::Content {
  arch::Arch config;
  std::vector<PECluster> pe;
  Memory edge_mem_left;
  Memory edge_mem_bottom;
};

Chip::Chip(arch::Arch c) {
  content = std::make_shared<Content>();
  content->config = c;
  for (int y = 0; y < c.y_num(); ++y) {
    for (int x = 0; x < c.x_num(); ++x) {
      content->pe.emplace_back(c.pe_cluster(), PEIndex{.x = x, .y = y});
    }
  }
  content->edge_mem_left = Memory(c.edge_mem_left());
  content->edge_mem_bottom = Memory(c.edge_mem_bottom());
}

arch::Arch Chip::config() const { return content->config; }

template <typename T> Cost Chip::execute(const Task<T> &t) {
  distribute_to_sram(t.b());
  // load input to PE 0, 0

  Cost c{.latency = 0, .energy = 0.f};

  return c;
}

template <typename T>
void Chip::loadA(const taco::Tensor<T> &a, int m_index, int y_index,
                 int k_tile_size) {
  std::vector<int> st = {m_index, y_index * k_tile_size};
  std::vector<int> size = {1, k_tile_size};
  auto subtensor = slice(a, st, size);
  content->edge_mem_left.writeTensor(subtensor, 0);

  printMatrix(subtensor);
}

// template <typename T>
// void Chip::loadAFromLeft() {
//
// }

template <typename T> void Chip::distribute_to_sram(const taco::Tensor<T> &x) {
  int tilex = (int)ceil(x.getDimension(0) / config().x_num());
  int tiley = (int)ceil(x.getDimension(1) / config().y_num());
  for (int i = 0; i < content->config.x_num(); ++i) {
    int stx = i * tilex;
    int sizex = std::min(tilex, x.getDimension(0) - stx);
    for (int j = 0; j < content->config.y_num(); ++j) {
      int sty = j * tiley;
      int sizey = std::min(tiley, x.getDimension(1) - sty);
      int idx = i * content->config.y_num() + j;
      PECluster &pec = content->pe.at(idx);
      taco::Tensor<T> subtensor = slice(x, {sty, stx}, {sizey, sizex});
      Memory mem = pec.mem();
      mem.writeTensor(subtensor, 0);
    }
  }
}

static std::pair<int, int> getDstPE(const std::pair<int, int> &pe0,
                                    const simu::Direction &dir) {
  switch (dir) {
  case Direction::N:
    return {pe0.first, pe0.second - 1};
  case Direction::S:
    return {pe0.first, pe0.second + 1};
  case Direction::W:
    return {pe0.first - 1, pe0.second};
  case Direction::E:
    return {pe0.first + 1, pe0.second};
  }
}

template <typename T>
size_t Chip::transfer(const std::pair<int, int> &pe0_index,
                      const simu::Direction &dir, int dstRID, int srcRID,
                      int data_num) {
  // get the dest pe
  auto pe1_index = getDstPE(pe0_index, dir);
  auto pec0 = getPECluster(pe0_index.first, pe0_index.second);
  auto pec1 = getPECluster(pe1_index.first, pe1_index.second);

  auto pe0 = pec0.pe(0);
  auto pe1 = pec1.pe(1);

  taco::Tensor<T> t({data_num * pe0.getRF().config().datawidth()}, taco::dense);
  t.pack();

  Cost c0 = pe0.getRF().readTensor(t, srcRID);
  Cost c1 = pe1.getRF().writeTensor(t, dstRID);

  return c0.latency + c1.latency;
}

const PECluster &Chip::getPECluster(int x, int y) const {
  return content->pe[y * config().x_num()  + x];
}

const std::vector<PECluster> &Chip::getPEArray() const { return content->pe; }

const Memory &Chip::getLeftMemory() const { return content->edge_mem_left; }
const Memory &Chip::getBottomMemory() const { return content->edge_mem_bottom; }

template <typename T> void Chip::print(bool sram, bool reg) const {
  std::cout << "=================== Chip =============\n";
  config().PrintDebugString();
  std::cout << "=================== Left Sram =============\n";
  content->edge_mem_left.print<T>(0, 5);
  std::cout << "=================== Bottom Sram =============\n";
  content->edge_mem_bottom.print<T>(0, 5);

  for (int i = 0; i < getPEArray().size(); ++i) {
    auto pec = getPEArray().at(i);
    std::cout << " ----------------- PECluster (x = " << pec.index().x
              << ", y =" << pec.index().y << ") -----------------\n";
    if (sram) {
      pec.mem().print<T>(0, 5);
    }

    for (int j = 0; j < pec.config().pe_num(); ++j) {
      std::cout << " ----------------- PE " << j << " -----------------\n";
      pec.pe(j).getRF().print<T>();
    }
  }
}

template void Chip::print<int>(bool, bool) const;
template void Chip::print<float>(bool, bool) const;
template void Chip::distribute_to_sram<int>(const taco::Tensor<int> &x);
template void Chip::distribute_to_sram<float>(const taco::Tensor<float> &x);
template Cost Chip::execute<int>(const Task<int> &t);
template Cost Chip::execute<float>(const Task<float> &t);
template void Chip::loadA<int>(const taco::Tensor<int> &a, int m_index,
                               int y_index, int k_tile_size);
template void Chip::loadA<float>(const taco::Tensor<float> &a, int m_index,
                                 int y_index, int k_tile_size);

} // namespace simu
