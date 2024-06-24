//
// Created by hyl on 15/05/24.
//

#include "glog/logging.h"
#include "src/config/event.h"
#include "src/event_gen.h"
#include "src/task.h"
#include "src/tensor.h"
#include "tests/utils.h"
#include "gtest/gtest.h"

using namespace simu;

class TestTask : public testing::Test {
protected:

  void SetUp() override {
    spmm = Task<float>::makeSPMM(20, 20, 20, 0.2, "spmm0");
    spmm.setTileSize(1, 5, 4);

    sddm = Task<int>::makeSDDM(10, 8, 4, 0.2, "sddm0");
  }

  void TearDown() override {

  }

  Task<float> spmm;
  Task<int> sddm;
};

TEST(event, basic) {
  LOG(INFO) << "this is a test";

  EventQueue queue(3);

  std::vector<std::vector<int64_t>> vec = {
      {2, 3, 5, 9, 10}, {4, 2, 11, 5, 6}, {3, 1, 2, 8, 5}};

  for (unsigned i = 0; i < vec.size(); ++i) {
    for (unsigned j = 0; j < vec.at(i).size(); ++j) {
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

TEST_F(TestTask, spmm) {
  LOG(INFO) << spmm.getKTileNum();
  LOG(INFO) << spmm.getKTileSize();

  EventQueue queue(spmm.getKTileNum());

  printMatrix(spmm.a());
  initEvents<float>(queue, spmm, 4);

  queue.print();

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

void CustomPrefix(std::ostream &s, const google::LogMessage &l, void*) {
  s << "[" << google::GetLogSeverityName(l.severity())[0] << "]"
    << std::setw(4) << 1900 + l.time().year()
    << std::setw(2) << 1 + l.time().month()
    << std::setw(2) << l.time().day()
    << ' '
    << std::setw(2) << l.time().hour() << ':'
    << std::setw(2) << l.time().min()  << ':'
    << std::setw(2) << l.time().sec() << "."
    << std::setw(6) << l.time().usec()
    << ' '
    << std::setfill(' ') << std::setw(5)
    << "[" << l.basename() << ':' << l.line() << "]";
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  FLAGS_stderrthreshold = google::INFO;
  google::InitGoogleLogging(argv[0]);
  google::InstallPrefixFormatter(CustomPrefix);
  return RUN_ALL_TESTS();
}