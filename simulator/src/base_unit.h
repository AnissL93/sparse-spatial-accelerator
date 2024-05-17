//
// Created by hyl on 15/05/24.
//

#ifndef SIMULATOR_BASE_UNIT_H
#define SIMULATOR_BASE_UNIT_H

#include <memory>

namespace simu {

template<typename SubClass>
class BaseUnit {
public:
  BaseUnit() {}

  void load_inst();

  void printInfo() {
    static_cast<SubClass>(this)->print();
  }
};

} // namespace simu

#endif // SIMULATOR_BASE_UNIT_H
