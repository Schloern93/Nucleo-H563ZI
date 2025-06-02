
#pragma once

#include <array>
#include <cstdint>

#include "interface_adc_channel.hpp"

struct ThermistorPoint {
  std::int16_t celsius;
  std::uint32_t resistance;
};

template <uint32_t Ressistor, size_t N, const std::array<ThermistorPoint, N> &curve>
class ThermistorSensor : public Interface_Sensor {
public:
  ThermistorSensor() = default;

  SensorData CalculateSensorData(uint32_t adcRawValue, uint32_t vRef, uint32_t adcResolution) const override {
    SensorData data{};
    data.unit = Units::CELCIUS;
    uint32_t resistance = GetResistenceFromAdcRawValue(adcRawValue, vRef, adcResolution);
    data.data = InterpolateTemperature(resistance);
    data.isValid = (data.data != -999);
    return data;
  }

private:
  uint32_t GetResistenceFromAdcRawValue(uint32_t adcRawValue, uint32_t vRef, uint32_t adcResolution) const {
    uint32_t voltage = ((adcRawValue * vRef) / adcResolution);
    uint32_t ressistorInOhm = ((voltage * Ressistor) / (vRef - voltage));
    return ressistorInOhm;
  }

  int16_t InterpolateTemperature(uint32_t resistance) const {
    int16_t interpolatedTemp = -999; // Default: Fehlerwert

    if(resistance <= curve.front().resistance) {
      interpolatedTemp = curve.front().celsius;
    } else if(resistance >= curve.back().resistance) {
      interpolatedTemp = curve.back().celsius;
    } else {
      for(size_t i = 0; i < curve.size() - 1; ++i) {
        const auto &lower = curve[i];
        const auto &upper = curve[i + 1];

        if(resistance >= lower.resistance && resistance <= upper.resistance) {
          uint32_t deltaRes = upper.resistance - lower.resistance;
          uint32_t deltaTemp = upper.celsius - lower.celsius;
          uint32_t deltaInput = resistance - lower.resistance;

          interpolatedTemp = lower.celsius + static_cast<int16_t>((deltaInput * deltaTemp) / deltaRes);
          break;
        }
      }
    }
    return interpolatedTemp;
  }
};

constexpr std::array<ThermistorPoint, 30U> semitec103ATCurve = {
    {{105, 857U},    {100, 973U},    {95, 1'108U},   {90, 1'266U},    {85, 1'451U},    {80, 1'668U},
     {75, 1'924U},   {70, 2'228U},   {65, 2'588U},   {60, 3'020U},    {55, 3'536U},    {50, 4'160U},
     {45, 4'911U},   {40, 5'827U},   {35, 6'940U},   {30, 8'313U},    {25, 10'000U},   {20, 12'000U},
     {15, 14'500U},  {10, 18'000U},  {5, 22'000U},   {0, 27'000U},    {-5, 33'000U},   {-10, 42'000U},
     {-15, 53'000U}, {-20, 67'000U}, {-25, 86'000U}, {-30, 111'000U}, {-35, 144'000U}, {-40, 188'000U}}};
