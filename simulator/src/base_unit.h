//
// Created by hyl on 15/05/24.
//

#ifndef SIMULATOR_BASE_UNIT_H
#define SIMULATOR_BASE_UNIT_H

#include <memory>

namespace simu {

class BaseUnit {
public:
  BaseUnit() {}

  void load_inst();

  virtual size_t exe() { return 0;};

};

} // namespace simu

#endif // SIMULATOR_BASE_UNIT_H
