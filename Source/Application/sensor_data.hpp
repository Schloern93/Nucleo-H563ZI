// sensor_data.hpp
#pragma once
#include <cstdint>

template <typename QuantityT> struct SensorData {
  QuantityT value; // z. B. units::DeciCelsius
  bool valid;
};
