
#pragma once

#include <cstdint>

enum class Units { NONE, CELCIUS, VOLTAGE };

struct SensorData {
  int32_t data{};
  Units unit{Units::NONE};
  bool isValid{false};
};

class Interface_Sensor {
public:
  virtual ~Interface_Sensor() = default;

  virtual SensorData CalculateSensorData(uint32_t adcRawValue, uint32_t vRef, uint32_t adcResolution) const = 0;
};

class Interface_SensorData {
public:
  virtual ~Interface_SensorData() = default;

  virtual SensorData GetSensorData() const = 0;
};