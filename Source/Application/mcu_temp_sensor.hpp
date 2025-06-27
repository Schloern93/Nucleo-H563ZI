
#pragma once

#include "interface_sensor.hpp"
#include "sensor_data.hpp"

class McuTempSensor : public Interface_Sensor<Units::Celsius> {
public:
  McuTempSensor() = default;

  SensorData<Units::Celsius> CalculateSensorData(uint32_t adcRawValue,
                                                 uint32_t vRef,
                                                 uint32_t adcResolution) const override {
    SensorData<Units::Celsius> data;
    const int32_t TS_CAL1 = static_cast<int32_t>(*reinterpret_cast<uint16_t *>(0x08FFF814UL));
    const int32_t TS_CAL2 = static_cast<int32_t>(*reinterpret_cast<uint16_t *>(0x08FFF818UL));
    const int32_t temp = ((TS_CAL2_TEMP - TS_CAL1_TEMP) * (adcRawValue - TS_CAL1)) / (TS_CAL2 - TS_CAL1) + TS_CAL1_TEMP;
    Units::Celsius celciusValue{static_cast<int16_t>(temp)};
    data.value = celciusValue;
    data.isValid = true;
    return data;
  }

private:
  static constexpr int32_t TS_CAL1_TEMP = 30;
  static constexpr int32_t TS_CAL2_TEMP = 130;
};
