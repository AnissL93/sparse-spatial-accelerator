//
// Created by hyl on 15/05/24.
//

#ifndef SIMULATOR_EVENT_H
#define SIMULATOR_EVENT_H

#include "src/config/event.pb.h"

namespace simu {

struct EventCmp {
  bool operator()(const event::Event& e1, const event::Event& e2);
};


} // namespace simu

#endif // SIMULATOR_EVENT_H
