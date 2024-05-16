//
// Created by hyl on 15/05/24.
//

#include "src/task.h"
#include "taco/util/fill.h"

namespace simu {

using namespace taco;

struct Task::Content {
  int m, n, k;
  float density;
  taco::Tensor<double> a, b, c;
  taco::Tensor<int> mask;
  std::string name;
};

Task Task::makeSDDM(int m, int n, int k, float dp, const std::string &name) {
  Format dense_2d({Dense, Dense});
  Format sparse({Sparse, Dense});
  Task t;
  t.content = std::make_shared<Content>();
  t.content->m = m;
  t.content->n = n;
  t.content->k = k;
  t.content->a = Tensor<double>({m, n}, dense_2d);
  t.content->a.setName(name + "_a");
  t.content->b = Tensor<double>({t.content->n, t.content->k}, dense_2d);
  t.content->b.setName(name + "_b");
  t.content->c = Tensor<double>({t.content->m, t.content->n}, sparse);
  t.content->c.setName(name + "_c");
  t.content->mask = Tensor<double>({t.content->m, t.content->n}, sparse);
  util::fillTensor(t.content->a, util::FillMethod::Dense);
  util::fillTensor(t.content->b, util::FillMethod::Dense);
  util::fillTensor(t.content->mask, util::FillMethod::Sparse, dp);
  t.content->density = dp;
  t.content->name = name;
  return t;
}

Task Task::makeSPMM(int m, int n, int k, float dp, const std::string &name) {
  Format csr({Dense, Sparse});
  Format dense_2d({Dense, Dense});
  Task t;
  t.content = std::make_shared<Content>();
  t.content->m = m;
  t.content->n = n;
  t.content->k = k;
  t.content->a = Tensor<double>({m, n}, csr);
  t.content->a.setName(name + "_a");
  t.content->b = Tensor<double>({t.content->n, t.content->k}, dense_2d);
  t.content->b.setName(name + "_b");
  t.content->c = Tensor<double>({t.content->m, t.content->n}, dense_2d);
  t.content->c.setName(name + "_c");
  t.content->density = dp;
  t.content->name = name;
  util::fillTensor(t.content->a, util::FillMethod::Dense, dp);
  util::fillTensor(t.content->b, util::FillMethod::Dense);
  return t;
}

void Task::compute() {
  IndexVar i, j, kk;
  content->c(i, j) += content->a(i, kk) * content->b(j, kk);
  content->c.compile();
  content->c.assemble();
  content->c.compute();
}

void Task::dump() const {
  std::cout << "=== Task: " << content->name
            << " density: " << content->density
            << " ===\n";
  std::cout << ">>> " << content->a << std::endl;
  std::cout << ">>> " << content->b << std::endl;
  std::cout << ">>> " << content->c << std::endl;
}

const taco::Tensor<double> &Task::a() const { return content->a; }

const taco::Tensor<double> &Task::b() const { return content->b; }

const taco::Tensor<double> &Task::c() const { return content->c; }

} // namespace simu
