#pragma once

#include "thread.hpp"
#include "FreeRTOSConfig.h"

#include "interface_adc_config.hpp"

class Task1 : public cpp_freertos::Thread {
public:
  Task1(Interface_AdcConfig &adcPollingConfigInit)
      : cpp_freertos::Thread("Task1", 256U, (configMAX_PRIORITIES - 1U)),
        adcPollingConfig(adcPollingConfigInit) {
    this->Start();
  }

  void Run() override {

    while(true) {
      // Do something
      Delay(500); // Delay for 1 second
      adcPollingConfig.UpdateAdcChannels();
    }
  }

private:
  Interface_AdcConfig &adcPollingConfig;
  uint32_t data[2];
};