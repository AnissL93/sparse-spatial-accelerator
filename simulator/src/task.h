//
// Created by hyl on 15/05/24.
//

#ifndef SIMULATOR_TASK_H
#define SIMULATOR_TASK_H

#include "taco/tensor.h"

namespace simu {

template <typename T> class Task {
public:
  Task() {}
  /**
   * Create spmm task with random number
   */
  static Task makeSPMM(int m, int n, int k, float dp, const std::string &name);
  /*
   * Create sddm task with random number
   */
  static Task makeSDDM(int, int, int, float, const std::string &);
  const taco::Tensor<T> &a() const;
  const taco::Tensor<T> &b() const;
  const taco::Tensor<T> &c() const;
  taco::Tensor<int> mask() const;
  void compute();
  void dump() const;
  void setTileSize(int m, int n, int k);
  int getMTileSize() const;
  int getNTileSize() const;
  int getKTileSize() const;
  int getMTileNum() const;
  int getNTileNum() const;
  int getKTileNum() const;

private:
  struct Content;
  std::shared_ptr<Content> content;
};

} // namespace simu

#endif // SIMULATOR_TASK_H
