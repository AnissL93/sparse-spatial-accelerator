//
// Created by hyl on 15/05/24.
//

#include "src/config/event.h"

namespace simu {

bool EventCmp::operator()(const event::Event &e1, const event::Event &e2) {
  return e1.clock() < e2.clock();
}

}  // namespace simu

