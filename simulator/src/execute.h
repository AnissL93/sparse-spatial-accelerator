//
// Created by hyl on 15/05/24.
//

#ifndef SIMULATOR_EXECUTE_H
#define SIMULATOR_EXECUTE_H

#include "src/chip.h"
#include "src/task.h"
#include "src/config/event.h"
#include <queue>

namespace simu {

class Executor {
public:
  void load_matrix();
  void run();

private:
  Task<double> task;
  Chip chip;
  std::priority_queue<event::Event, std::vector<event::Event>, EventCmp> event;
};

}  // namespace simu

#endif // SIMULATOR_EXECUTE_H
