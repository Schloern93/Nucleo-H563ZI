#pragma once

#include "thread.hpp"
#include "FreeRTOSConfig.h"

#include "interface_adc_channel.hpp"

class Task2 : public cpp_freertos::Thread {
public:
  Task2(Interface_AdcChannelGetter &ch1Init, Interface_AdcChannelGetter &ch2Init)
      : cpp_freertos::Thread("Task2", 50U, (configMAX_PRIORITIES - 1U)),
        ch1(ch1Init),
        ch2(ch2Init) {
    this->Start();
  }

  void Run() override {
    // Task implementation goes here
    while(true) {
      // Do something
      Delay(1000); // Delay for 1 second
      data[0] = ch1.GetVoltageMv();
      data[1] = ch2.GetVoltageMv();
    }
  }

private:
  Interface_AdcChannelGetter &ch1;
  Interface_AdcChannelGetter &ch2;
  uint32_t data[2] = {0, 0};
};