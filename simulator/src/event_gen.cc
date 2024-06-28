//
// Created by hyl on 27/05/24.
//

#include "event_gen.h"

using namespace event;

static event::Event getNextEvent(const Event &e) {
  switch (e.type()) {
  case event::PE_VMS: {

  } break;
  }
}

void simu::passEventN2S(simu::EventQueue &queue, int pe_index) {
  CHECK_LT(pe_index + 1, queue.getPENum()) << "PE index too large";
  auto &upper_events = queue.getEvent(pe_index);
  auto &lower_events = queue.getEvent(pe_index + 1);

  for (auto iter = upper_events.begin(); iter != upper_events.end(); ++iter) {
    switch (iter->type()) {
    case event::PE_VMS: {

    } break;
    case event::PE_ST: {

    } break;
    case event::PE_LD: {

    } break;
    case event::PASS: {

    } break;
    case event::PE_ACC: {

    } break;
    default:
      break;
    }
  }
}
