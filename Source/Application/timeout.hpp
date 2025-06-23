/********************************************************************
Copyright (c) Truma Geraetetechnik, Putzbrunn. All rights reserved.
********************************************************************/
#pragma once

#include "interface_timeout.hpp"
#include "ticks.hpp"

#include <limits>
#include <type_traits>

class Timeout : public Interface_Timeout {

public:
  static_assert(std::is_unsigned<TickType_t>::value, "Storage type of timeout class must be unsigned integral");

  Timeout(const std::uint32_t new_timeout) {
    StartMs(new_timeout);
  }

  Timeout()
      : startValue(0U),
        timeoutValue(0U) {
  }

  void StartMs(const std::uint32_t new_timeout) override {
    startValue = cpp_freertos::Ticks::GetTicks();
    timeoutValue = cpp_freertos::Ticks::MsToTicks(new_timeout);
  }

  bool IsExpired() const override {
    bool ret;
    if(timeoutValue != 0U) {
      const TickType_t elapsed = Elapsed();
      ret = (elapsed > timeoutValue);
    } else {
      // Timeout not set or set to zero (timeout == 0U)
      ret = true;
    }
    return ret;
  }

  uint32_t ElapsedMs() const override {
    return cpp_freertos::Ticks::TicksToMs(Elapsed());
  }

private:
  TickType_t startValue;
  TickType_t timeoutValue;

  TickType_t Elapsed() const {
    const TickType_t current = cpp_freertos::Ticks::GetTicks();
    const TickType_t elapsed = (current >= startValue)
                                   ? (current - startValue)
                                   : ((std::numeric_limits<TickType_t>::max() - startValue) + current);
    static_assert(sizeof(uint32_t) == sizeof(TickType_t));
    return elapsed;
  }
};
