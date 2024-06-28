//
// Created by hyl on 15/05/24.
//

#ifndef SIMULATOR_EVENT_H
#define SIMULATOR_EVENT_H

#include "src/config/event.pb.h"
#include <list>
#include <queue>

namespace simu {

struct EventCmp {
  bool operator()(const event::Event &e1, const event::Event &e2);
};

using Event = event::Event;

void printEvent(const Event &e);

std::string toString(const Event &e);

Event createEvent(const event::EventType &type, int clock, int dur, int rid);

/*
 * Events for all PEs
 */
class EventQueue {
public:
  EventQueue(int pe_num) { event_queue_.resize(pe_num); }
  void pushEvent(int peIdx, const Event &e);
  // Insert e[40-50] at clock will cut the event that is at clock to be two
  // parts.
  void insertEvent(int peIdx, const Event &new_e, int clock);

  void updateClock(int64_t st = 0);

  void print() const;

  // Get event list in pe_index.
  std::list<Event> &getEvent(int pe_index);

  Event& getEventOfClock(int pe_index, unsigned clock);

  int getPENum() const { return event_queue_.size(); }

private:
  // PE<list of events>
  std::vector<std::list<Event>> event_queue_;
};

} // namespace simu

#endif // SIMULATOR_EVENT_H
