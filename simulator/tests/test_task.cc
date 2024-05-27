//
// Created by hyl on 15/05/24.
//

#include "src/config/event.h"
#include "src/event_gen.h"
#include "src/task.h"
#include "src/tensor.h"
#include "tests/utils.h"
#include "gtest/gtest.h"

using namespace simu;

TEST(event, basic) {
  EventQueue queue(3);

  std::vector<std::vector<int64_t>> vec = {
      {2, 3, 5, 9, 10}, {4, 2, 11, 5, 6}, {3, 1, 2, 8, 5}};

  for (int i = 0; i < vec.size(); ++i) {
    for (int j = 0; j < vec.at(i).size(); ++j) {
      event::Event event1;
      event1.set_type(event::PE_VMS);
      event1.set_clock(0);
      event1.set_dur_clock(vec[i][j]);
      event1.set_row_idx(j);
      queue.pushEvent(i, event1);
    }
  }

  queue.updateClock();
  queue.print();

  // insert store for the first
  event::Event e;
  e.set_row_idx(0);
  e.set_clock(2);
  e.set_dur_clock(1);
  e.set_type(event::PASS);

  queue.insertEvent(0, e, 2);
  queue.print();
  queue.updateClock();
  queue.print();
  FAIL();
}

TEST(task, genEvent) {
  Task<float> spmm = Task<float>::makeSPMM(5, 5, 4, 0.6, "spmm0");
  spmm.setTileSize(1, 5, 4);
  std::cout << spmm.getKTileSize();
  std::cout << spmm.getKTileNum();
  EventQueue queue(3);
  initEvents<float>(queue, spmm);

  FAIL();
}

TEST(task, spmm) {
  Task<float> spmm = Task<float>::makeSPMM(10, 8, 4, 0.2, "spmm0");
  spmm.compute();
  Tensor<float> ref(spmm.c().getDimensions());
  matmulRef(spmm.a(), spmm.b(), ref);
  ASSERT_TRUE(equals(spmm.c(), ref));
}

TEST(task, sddm) {
  {
    Task<float> sddm = Task<float>::makeSDDM(10, 8, 4, 0.5, "spmm0");
    sddm.compute();
    Tensor<float> ref("ref", sddm.c().getDimensions());
    matmulRef(sddm.a(), sddm.b(), ref, sddm.mask());
    ASSERT_TRUE(equals(sddm.c(), ref));
  }

  {
    Task<int> sddm = Task<int>::makeSDDM(10, 8, 4, 0.5, "spmm0");
    sddm.compute();
    Tensor<int> ref("ref", sddm.c().getDimensions());
    matmulRef<int>(sddm.a(), sddm.b(), ref, sddm.mask());
    ASSERT_TRUE(equals(sddm.c(), ref));

    printMatrix(ref);
    int total_size = 0;
    for (auto iter = ref.beginTyped<int>(); iter != ref.endTyped<int>();
         ++iter) {
      std::cout << iter->second << "\n";
      total_size++;
    }
    std::cout << total_size << std::endl;
    FAIL();
  }
}
