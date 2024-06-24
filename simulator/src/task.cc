//
// Created by hyl on 15/05/24.
//

#include "src/task.h"
#include "src/tensor.h"

namespace simu {

using namespace taco;

template <typename T> struct Task<T>::Content {
  int m, n, k;
  float density;
  taco::Tensor<T> a, b, c;
  taco::Tensor<int> mask;
  std::string name;

  // tile size
  int m_tile, n_tile, k_tile;
};

template <typename T>
Task<T> Task<T>::makeSDDM(int m, int n, int k, float dp,
                          const std::string &name) {
  Format dense_2d({Dense, Dense});
  Format sparse({Sparse, Sparse});
  Task t;
  t.content = std::make_shared<Content>();
  t.content->m = m;
  t.content->n = n;
  t.content->k = k;
  t.content->a = Tensor<T>({m, k}, dense_2d);
  t.content->a.setName(name + "_a");
  t.content->b = Tensor<T>({n, k}, dense_2d);
  t.content->b.setName(name + "_b");
  t.content->c = Tensor<T>({m, n}, dense_2d);
  t.content->c.setName(name + "_c");
  t.content->mask = Tensor<int>({t.content->m, t.content->n}, sparse);
  t.content->density = dp;
  t.content->name = name;
  fillRandom<T>(t.content->a, FillMethod::Dense);
  fillRandom<T>(t.content->b, FillMethod::Dense);
  if (t.content->mask.getOrder() > 0) {
    generateSparseMatrix<int>(t.content->mask, dp);
//    fillRandom<int>(t.content->mask, FillMethod::Sparse, dp, true);
  }
  return t;
}

template <typename T> void Task<T>::setTileSize(int m, int n, int k) {
  content->m_tile = m;
  content->n_tile = n;
  content->k_tile = k;
}

template <typename T> int Task<T>::getMTileSize() const {
  return content->m_tile;
}
template <typename T> int Task<T>::getNTileSize() const {
  return content->n_tile;
}
template <typename T> int Task<T>::getKTileSize() const {
  return content->k_tile;
}
template <typename T> int Task<T>::getMTileNum() const {
  return static_cast<int>(ceil(content->m / content->m_tile));
}
template <typename T> int Task<T>::getNTileNum() const {
  return static_cast<int>(ceil(content->n / content->n_tile));
}
template <typename T> int Task<T>::getKTileNum() const {
  return static_cast<int>(ceil(content->k / content->k_tile));
}

template <typename T>
Task<T> Task<T>::makeSPMM(int m, int n, int k, float dp,
                          const std::string &name) {
  Format csr({Sparse, Sparse});
  Format dense_2d({Dense, Dense});
  Task t;
  t.content = std::make_shared<Content>();
  t.content->m = m;
  t.content->n = n;
  t.content->k = k;
  t.content->a = Tensor<T>({m, k}, csr);
  t.content->a.setName(name + "_a");
  t.content->b = Tensor<T>({n, k}, dense_2d);
  t.content->b.setName(name + "_b");
  t.content->c = Tensor<T>({m, n}, sparse);
  t.content->c.setName(name + "_c");
  t.content->c.pack();
  t.content->density = dp;
  t.content->name = name;
//  fillRandom<T>(t.content->a, FillMethod::Sparse, dp);
  generateSparseMatrix<T>(t.content->a, dp);
  fillRandom<T>(t.content->b, FillMethod::Dense);
  return t;
}

template <typename T> void Task<T>::compute() {
  IndexVar i, j, kk;
  if (mask().getOrder() == 0) {
    content->c(i, j) += content->a(i, kk) * content->b(j, kk);
  } else {
    content->c(i, j) +=
        content->a(i, kk) * content->b(j, kk) * content->mask(i, j);
  }
  content->c.compile();
  content->c.assemble();
  content->c.compute();
  content->c.pack();
}

template <typename T> void Task<T>::dump() const {
  std::cout << "=== Task: " << content->name << " density: " << content->density
            << " ===\n";
  printMatrix(content->a);
  printMatrix(content->b);
  printMatrix(content->c);
}

template <typename T> const taco::Tensor<T> &Task<T>::a() const {
  return content->a;
}

template <typename T> const taco::Tensor<T> &Task<T>::b() const {
  return content->b;
}

template <typename T> const taco::Tensor<T> &Task<T>::c() const {
  return content->c;
}

template <typename T> taco::Tensor<int> Task<T>::mask() const {
  return content->mask;
}

template class Task<int>;
template class Task<float>;

} // namespace simu
