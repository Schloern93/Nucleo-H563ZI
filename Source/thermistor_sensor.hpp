
#pragma once

#include <array>

#include "interface_adc_channel.hpp"

struct ThermistorPoint {
  std::int16_t celsius;
  std::uint32_t resistance;
};

template <uint32_t N> class ThermistorSensor : public Interface_Sensor {
public:
  ThermistorSensor(Interface_AdcChannel &adcChannelInit,
                   const std::array<ThermistorPoint, N> &curveInit,
                   const uint32_t ressistorInit)
      : adcChannel(adcChannelInit),
        curve(curveInit),
        ressistor(ressistorInit) {
  }

  SensorData GetSensorData() override {
    SensorData data;
    data.unit = Units::CELCIUS;

    // Read the ADC value
    const auto adcValueMv = adcChannel.GetAdcRawValue();

    // Convert ADC value to resistance
    const auto resistance = (ressistor * adcValueMv) / (3300U - adcValueMv);

    // Find the corresponding temperature from the curve
    for(const auto &point : curve) {
      if(point.resistance <= resistance) {
        data.data = point.celsius;
        data.isValid = true;
        break;
      }
    }
    return data;
  }

private:
  Interface_AdcChannel &adcChannel;
  const std::array<ThermistorPoint, N> &curve;
  const uint32_t ressistor;
};

constexpr std::array<ThermistorPoint, 30U> semitec103ATCurve = {
    {{105, 857U},    {100, 973U},    {95, 1'108U},   {90, 1'266U},    {85, 1'451U},    {80, 1'668U},
     {75, 1'924U},   {70, 2'228U},   {65, 2'588U},   {60, 3'020U},    {55, 3'536U},    {50, 4'160U},
     {45, 4'911U},   {40, 5'827U},   {35, 6'940U},   {30, 8'313U},    {25, 10'000U},   {20, 12'000U},
     {15, 14'500U},  {10, 18'000U},  {5, 22'000U},   {0, 27'000U},    {-5, 33'000U},   {-10, 42'000U},
     {-15, 53'000U}, {-20, 67'000U}, {-25, 86'000U}, {-30, 111'000U}, {-35, 144'000U}, {-40, 188'000U}}};
