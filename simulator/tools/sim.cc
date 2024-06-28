//
// Created by hyl on 20/06/24.
//

#include "tools/sim.h"
#include "gflags.h"
#include "glog/logging.h"
#include "src/chip.h"
#include "src/task.h"

DEFINE_string(algo, "spmm",
              "Type of sparse matrix multiplication: spmm, sddm, diag");
DEFINE_double(density, 0.5,
              "The percent of non-zero in random matrix if no file is given");
DEFINE_int32(m, 10, "m size");
DEFINE_int32(n, 10, "n size");
DEFINE_int32(k, 10, "k size");

// todo
//DEFINE_string(mata, "mata.prototxt", "data file for matrix A.");
//DEFINE_string(matb, "matb.prototxt", "data file for matrix B.");
DEFINE_string(dtype, "float", "Datatype of the matrix multiplication: float, "
                              "int32");

int main(int argc, char **argv) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  double density = FLAGS_density;
  if (FLAGS_algo == "spmm") {
    if (FLAGS_dtype == "float") {
      auto task = simu::Task<float>::makeSPMM(FLAGS_m, FLAGS_n, FLAGS_k,
                                              density,
                                              "spmm_float");
      task.dump();
    } else if (FLAGS_dtype == "int32") {
      auto task = simu::Task<int32_t>::makeSPMM(FLAGS_m, FLAGS_n, FLAGS_k,
                                              density,
                                              "spmm_int32");
      task.dump();
    } else {
      LOG(FATAL)  << "Not support dtype " << FLAGS_dtype;
      return -1;
    }
  } else if (FLAGS_algo == "sddm") {
    LOG(FATAL) << "not support";
  } else if (FLAGS_algo == "diag") {
    LOG(FATAL) << "not support";
  }
  return 0;
}
