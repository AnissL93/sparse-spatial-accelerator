//
// Created by hyl on 15/05/24.
//

#include "src/config/event.h"
#include <iomanip>
#include <sstream>

namespace simu {

bool EventCmp::operator()(const event::Event &e1, const event::Event &e2) {
  return e1.clock() < e2.clock();
}

void EventQueue::pushEvent(int peIdx, const simu::Event &e) {
  event_queue_[peIdx].push_back(e);
}

void EventQueue::insertEvent(int peIdx, const simu::Event &new_e, int clock) {
  // ea
  // --> ea_first eb ea_rest

  // find the event to be replaced

  auto &l = event_queue_[peIdx];
  std::list<Event>::iterator replacedEvent;
  for (auto iter = l.begin(); iter != l.end(); ++iter) {
    if (clock >= iter->clock() && clock < iter->clock() + iter->dur_clock()) {
      // split this event to two events, and insert the new event in between.
      replacedEvent = iter;
      break;
    }
  }

  if (clock == replacedEvent->clock()) {
    l.insert(replacedEvent, new_e);
    return;
  }

  // split e into two events
  Event e2;
  e2.CopyFrom(*replacedEvent);
  int64_t e1_dur = clock - replacedEvent->clock();
  int64_t e2_dur = replacedEvent->dur_clock() - e1_dur;

  replacedEvent->set_dur_clock(e1_dur);

  e2.set_clock(clock + new_e.dur_clock());
  e2.set_dur_clock(e2_dur);

  std::cout << "insert " << toString(*replacedEvent) << std::endl;
  replacedEvent = std::next(replacedEvent);
  std::cout << "insert " << toString(new_e) << std::endl;
  l.insert(replacedEvent, new_e);
  std::cout << "insert " << toString(e2) << std::endl;
  //  replacedEvent = std::next(replacedEvent);
  l.insert(replacedEvent, e2);
}

void EventQueue::updateClock(int64_t st) {
  for (int i = 0; i < event_queue_.size(); ++i) {
    int64_t cur_clock = st;
    for (auto it = event_queue_.at(i).begin(); it != event_queue_.at(i).end();
         ++it) {
      it->set_clock(cur_clock);
      cur_clock += it->dur_clock();
    }
  }

  // remove event if dur is 0
}

void printEvent(const Event &e) { std::cout << toString(e) << std::endl; }

std::string toString(const Event &e) {
  std::stringstream ss;
  switch (e.type()) {
  case event::PE_VMS:
    ss << "vms";
    break;
  case event::PE_LD:
    ss << "ld ";
    break;
  case event::PE_ST:
    ss << "st ";
    break;
  case event::PASS:
    ss << "pas";
    break;
  default:
    break;
  }

  ss << "[" << e.clock() << "+" << e.dur_clock() << "="
     << e.clock() + e.dur_clock() << "]@" << e.row_idx();
  return ss.str();
}

void EventQueue::print() const {
  std::cout << std::fixed;
  for (int i = 0; i < event_queue_.size(); ++i) {
    auto &l = event_queue_.at(i);
    std::cout << "PE" << i << " ";
    for (auto it = l.begin(); it != l.end(); ++it) {
      std::cout << std::left << std::setw(20) << toString(*it);
    }
    std::cout << "\n";
  }
}

std::list<Event> &EventQueue::getEvent(int pe_index) {
  return event_queue_.at(pe_index);
}

Event &EventQueue::getEventOfClock(int pe_index, unsigned clock) {
  auto &event_list = event_queue_.at(pe_index);
  for (auto iter = event_list.begin(); iter != event_list.end(); ++iter) {
    auto st = iter->clock();
    auto ed = iter->clock() + iter->dur_clock();
    if (clock >= st && clock < ed) {
      return *iter;
    }
  }
  LOG(FATAL) << "No event found for clock " << clock;
}

Event createEvent(const event::EventType &type, int clock, int dur, int rid) {
  Event e;
  e.set_type(type);
  e.set_dur_clock(dur);
  e.set_clock(clock);
  e.set_row_idx(rid);
  return e;
}

} // namespace simu
