#pragma once

#include "thread.hpp"

#include "timeout.hpp"

#include "stm32h5xx_hal_gpio.h"

class Task3 : public cpp_freertos::Thread {
public:
  Task3()
      : cpp_freertos::Thread("Task3", 120U, (configMAX_PRIORITIES - 1U)) {
    this->Start();
  }

  void Run() override {
    // Task implementation goes here
    while(true) {
      uint32_t reloadValue;

      if(timer.IsExpired()) {
        timer.StartMs(3000U);
        reloadValue = SysTick->LOAD;
        tickCtr = SysTick->CTRL;
        sysTickvalue = SysTick->VAL;
      }
      //   vTaskDelay(1000U);
      int testValue = 0;
      testDone = true;
      HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_0);
    }
  }

private:
  Timeout timer;
  bool testDone{false};
  uint32_t tickCounter{0U};
  uint32_t tickCtr{0U};
  uint32_t sysTickvalue{0U};
};