
#pragma once

#include <cstdint>

#include "sensor_data.hpp"

template <typename QuantityT> class Interface_Sensor {
public:
  virtual ~Interface_Sensor() = default;

  virtual SensorData<QuantityT> CalculateSensorData(uint32_t adcRawValue,
                                                    uint32_t vRef,
                                                    uint32_t adcResolution) const = 0;
};

template <typename QuantityT> class Interface_SensorData {
public:
  virtual ~Interface_SensorData() = default;

  virtual SensorData<QuantityT> GetSensorData() const = 0;
};