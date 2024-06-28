//
// Created by hyl on 27/05/24.
//

#ifndef SIMULATOR_EVENT_GEN_H
#define SIMULATOR_EVENT_GEN_H

#include "src/config/event.h"
#include "src/task.h"
#include "src/tensor.h"
#include "src/chip.h"
#include "glog/logging.h"
#include <queue>

namespace simu {

// Initialize events for the first line of PEs.
template <typename T>
void initEvents(EventQueue& q, const Task<T> &t, unsigned vec_lane_size) {
  for (int i = 0; i < t.getKTileNum(); ++i) {
    for (int j = 0; j < t.getMTileNum(); ++j) {
      taco::Tensor<T> suba = slice(t.a(), {j * t.getMTileSize(), i * t
                                                                       .getKTileSize()},
                        {t.getMTileSize(), t.getKTileSize()});

      unsigned none_zero = getNumOfNoneZero<T>(suba);
      unsigned clock_of_compute = static_cast<unsigned >(std::ceil((float) none_zero /
                                                         (float)vec_lane_size));
//      if (none_zero != 0) {
        event::Event event1;
        event1.set_type(event::PE_VMS);
        event1.set_clock(0);
        event1.set_dur_clock(clock_of_compute);
        event1.set_row_idx(j);
        q.pushEvent(i, event1);
//      }
    }
  }
  q.updateClock();
}

void passEventN2S(EventQueue& q, int pe_idx);

} // namespace simu

#endif // SIMULATOR_EVENT_GEN_H
