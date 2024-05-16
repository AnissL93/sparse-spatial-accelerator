//
// Created by hyl on 15/05/24.
//

#ifndef SIMULATOR_TASK_H
#define SIMULATOR_TASK_H

#include "taco/tensor.h"

namespace simu {

class Task {
public:
  Task() {}
  static Task makeSPMM(int, int, int, float, const std::string&);
  static Task makeSDDM(int, int, int, float, const std::string&);
  const taco::Tensor<double>& a() const;
  const taco::Tensor<double>& b() const;
  const taco::Tensor<double>& c() const;
  void compute();
  void dump() const;
private:
  struct Content;
  std::shared_ptr<Content> content;
};


} // namespace simu

#endif // SIMULATOR_TASK_H
