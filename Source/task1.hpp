#pragma once

#include "thread.hpp"
#include "FreeRTOSConfig.h"

#include "adc_test.hpp"

class Task1 : public cpp_freertos::Thread {
public:
  Task1(ADC_Test &adc_test)
      : cpp_freertos::Thread("Task1", 256U, (configMAX_PRIORITIES - 1U)),
        adc_test(adc_test) {
    this->Start();
  }

  void Run() override {
    // Task implementation goes here
    adc_test.StartAdcWithDma();
    while(true) {
      // Do something
      Delay(1000); // Delay for 1 second
      uint32_t *data = adc_test.getData();
    }
  }

private:
  ADC_Test &adc_test;
};