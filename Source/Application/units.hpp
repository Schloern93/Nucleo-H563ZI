
#pragma once

#include <ratio>

#include "quanity.hpp"

namespace Units {

// Unit tags for different physical quantities
struct UnitCelsius {};
struct UnitKelvin {};
struct UnitVolt {};

// Unit Celsius
using Celsius = Quantity<UnitCelsius, int16_t>;                        // 1 °C
using DeciCelsius = Quantity<UnitCelsius, int16_t, std::ratio<1, 10>>; // 0,1 °C
// Unit Kelvin
using Kelvin = Quantity<UnitKelvin, int16_t>;                        // 1 K
using DeciKelvin = Quantity<UnitKelvin, int16_t, std::ratio<1, 10>>; // 0,1 K
// Unit Volt
using Volt = Quantity<UnitVolt, int32_t>;                  // 1 V
using MilliVolt = Quantity<UnitVolt, int32_t, std::milli>; // 1 mV

// Conversion functions
constexpr DeciKelvin toKelvin(Celsius c) {
  return DeciKelvin(c.getRawValue() + 273);
}
constexpr DeciKelvin toKelvin(DeciCelsius c) {
  return DeciKelvin(c.getRawValue() + 2731);
}
constexpr DeciCelsius toCelsius(Kelvin k) {
  return DeciCelsius(k.getRawValue() - 273);
}
constexpr DeciCelsius toCelsius(DeciKelvin k) {
  return DeciCelsius(k.getRawValue() - 2731);
}

} // namespace Units