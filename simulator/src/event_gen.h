//
// Created by hyl on 27/05/24.
//

#ifndef SIMULATOR_EVENT_GEN_H
#define SIMULATOR_EVENT_GEN_H

#include "src/config/event.h"
#include "src/task.h"
#include "src/tensor.h"
#include "glog/logging.h"
#include <queue>

namespace simu {

// Initialize events for the first line of PEs.
template <typename T>
void initEvents(const EventQueue& q, const Task<T> &t) {
  for (int i = 0; i < t.getKTileNum(); ++i) {
    for (int j = 0; j < t.getMTileNum(); ++j) {
      taco::Tensor<T> suba = slice(t.a(), {j * t.getMTileSize(), i * t
                                                                       .getKTileSize()},
                        {t.getMTileSize(), t.getKTileSize()});
      auto nz = suba.getStorage().getValues().getSize();

      LOG(INFO) << "nz of tile " << i << " " << j << " is " << nz << std::endl;
      LOG(INFO) << suba << std::endl;
    }
  }
}

} // namespace simu

#endif // SIMULATOR_EVENT_GEN_H
