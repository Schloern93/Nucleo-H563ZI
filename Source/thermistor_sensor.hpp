
#pragma once

#include <array>

#include "interface_adc_channel.hpp"

struct ThermistorPoint {
  std::int16_t celsius;
  std::uint32_t resistance;
};

template <uint32_t Ressistor, typename CurveType> class ThermistorSensor : public Interface_Sensor {
public:
  ThermistorSensor() = default;

  SensorData CalculateSensorData(uint32_t adcRawValue, uint32_t vRef, uint32_t adcResolution) const override {
    SensorData data{};
    data.unit = Units::CELCIUS;

    const std::uint16_t digits = adcRawValue;
    std::uint16_t temperature;
    bool valid = false;
    const auto it =
        std::adjacent_find(std::begin(digit_points), std::end(digit_points), [digits](const auto &a, const auto &b) {
          return (digits >= a.digits && digits <= b.digits);
        });
    if(it != std::end(digit_points)) {
      ThermistorPointDigits a = *it;
      ThermistorPointDigits b = *(it + 1);

      const std::int32_t temperature_step =
          static_cast<std::int32_t>(b.kelvin10) - static_cast<std::int32_t>(a.kelvin10);
      const std::int32_t digits_step = static_cast<std::int32_t>(b.digits) - static_cast<std::int32_t>(a.digits);
      const std::int32_t digits_offset = static_cast<std::int32_t>(digits) - static_cast<std::int32_t>(a.digits);

      const std::int32_t temperature_offset = Util::DivAndRound(digits_offset * temperature_step, digits_step);
      const std::int32_t temperature_int32 = static_cast<std::int32_t>(a.kelvin10) + temperature_offset;

      temperature = static_cast<std::uint16_t>(static_cast<std::uint32_t>(temperature_int32));
      valid = true;
    } else if(digits <= digit_points.front().digits) {
      temperature = digit_points.front().kelvin10;
    } else {
      temperature = digit_points.back().kelvin10;
    }
    return data;
  }

private:
  const CurveType &curve;
  const Ressistor ressistor;
};

constexpr std::array<ThermistorPoint, 30U> semitec103ATCurve = {
    {{105, 857U},    {100, 973U},    {95, 1'108U},   {90, 1'266U},    {85, 1'451U},    {80, 1'668U},
     {75, 1'924U},   {70, 2'228U},   {65, 2'588U},   {60, 3'020U},    {55, 3'536U},    {50, 4'160U},
     {45, 4'911U},   {40, 5'827U},   {35, 6'940U},   {30, 8'313U},    {25, 10'000U},   {20, 12'000U},
     {15, 14'500U},  {10, 18'000U},  {5, 22'000U},   {0, 27'000U},    {-5, 33'000U},   {-10, 42'000U},
     {-15, 53'000U}, {-20, 67'000U}, {-25, 86'000U}, {-30, 111'000U}, {-35, 144'000U}, {-40, 188'000U}}};
