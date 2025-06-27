
#pragma once

#include "units.hpp"

template <typename QuantityT> struct SensorData {
  QuantityT value;
  bool isValid;
};
