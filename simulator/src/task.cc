//
// Created by hyl on 15/05/24.
//

#include "src/task.h"
#include "taco/util/fill.h"

namespace simu {

using namespace taco;

struct Task::Content {
  int m_, n_, k_;
  float density_;
  taco::Tensor<double> a_, b_, c_;
  std::string name;
};

Task Task::makeSPMM(int m, int n, int k, float dp, const std::string &name) {
  Format csr({Dense, Sparse});
  Format dense_2d({Dense, Dense});
  Task t;
  t.content = std::make_shared<Content>();
  t.content->m_ = m;
  t.content->n_ = n;
  t.content->k_ = k;
  t.content->a_ = Tensor<double>({m, n}, csr);
  t.content->a_.setName(name + "_a");
  t.content->b_ = Tensor<double>({t.content->n_, t.content->k_}, dense_2d);
  t.content->b_.setName(name + "_b");
  t.content->c_ = Tensor<double>({t.content->m_, t.content->n_}, dense_2d);
  t.content->c_.setName(name + "_c");
  t.content->density_ = dp;
  t.content->name = name;

  util::fillTensor(t.content->a_, util::FillMethod::Dense, dp);
  util::fillTensor(t.content->b_, util::FillMethod::Dense);

  return t;
}

void Task::dump() const {
  std::cout << "=== Task: " << content->name << " density: "
            << content->density_ << " ===\n";
  std::cout << ">>> " << content->a_ << std::endl;
  std::cout << ">>> " << content->b_ << std::endl;
  std::cout << ">>> " << content->c_ << std::endl;
}

} // namespace simu
