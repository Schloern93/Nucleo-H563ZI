
#pragma once

#include <ratio>

#include "quantity.hpp"

namespace units {

// ─── Unit-Tags ──────────────────────────────────────────────────────────
struct Celsius {};
struct Kelvin {};
struct VoltTag {};
// …

// ─── Alias-Typen (Name = Skala + Einheit) ──────────────────────────────
using DeciCelsius = Quantity<Celsius, int16_t, std::ratio<1, 10>>; // 0,1 °C
using DeciKelvin = Quantity<Kelvin, int16_t, std::ratio<1, 10>>;   // 0,1 K
using MilliVolt = Quantity<VoltTag, int32_t, std::milli>;          // 1 mV
using Volt = Quantity<VoltTag, int32_t>;                           // 1 V

// ─── Einheiten­wechsel mit Offset ──────────────────────────────────────
constexpr DeciKelvin toKelvin(DeciCelsius c) {
  return DeciKelvin{c.raw() + 2731};
}
constexpr DeciCelsius toCelsius(DeciKelvin k) {
  return DeciCelsius{k.raw() - 2731};
}

} // namespace units