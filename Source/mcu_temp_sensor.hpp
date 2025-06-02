
#pragma once

#include "interface_sensor.hpp"

class McuTempSensor : public Interface_Sensor {
public:
  McuTempSensor() = default;

  SensorData CalculateSensorData(uint32_t adcRawValue, uint32_t vRef, uint32_t adcResolution) const override {
    SensorData data{};
    data.unit = Units::CELCIUS;
    data.isValid = true;
    data.data = ((TS_CAL2_TEMP - TS_CAL1_TEMP) * (adcRawValue - TS_CAL1)) / (TS_CAL2 - TS_CAL1) + TS_CAL1_TEMP;
    return data;
  }

private:
  const int32_t TS_CAL1 = static_cast<int32_t>(*reinterpret_cast<uint16_t *>(0x08FFF814UL));
  const int32_t TS_CAL2 = static_cast<int32_t>(*reinterpret_cast<uint16_t *>(0x08FFF818UL));
  static constexpr int32_t TS_CAL1_TEMP = 30;
  static constexpr int32_t TS_CAL2_TEMP = 130;
};
