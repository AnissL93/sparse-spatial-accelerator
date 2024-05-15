//
// Created by hyl on 15/05/24.
//

#include "src/task.h"
#include "gtest/gtest.h"

using namespace simu;

TEST(task, spmm) {
  Task spmm = Task::makeSPMM(4, 4, 4, 0.2, "spmm0");
  spmm.dump();
  FAIL();
}

