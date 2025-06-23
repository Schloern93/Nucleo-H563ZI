#pragma once

#include "thread.hpp"

#include "interface_sensor.hpp"

#include "timeout.hpp"

class Task2 : public cpp_freertos::Thread {
public:
  Task2(Interface_SensorData &externalTempSensorInit, Interface_SensorData &internalTempSensorInit)
      : cpp_freertos::Thread("Task2", 120U, (configMAX_PRIORITIES - 1U)),
        externalTempSensor(externalTempSensorInit),
        internalTempSensor(internalTempSensorInit) {
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
      // vTaskDelay(1000U);
      data[0] = externalTempSensor.GetSensorData();
      data[1] = internalTempSensor.GetSensorData();
    }
  }

private:
  Interface_SensorData &externalTempSensor;
  Interface_SensorData &internalTempSensor;
  Timeout timer;
  SensorData data[2]{};
  bool testDone{false};
  uint32_t tickCounter{0U};
  uint32_t tickCtr{0U};
  uint32_t sysTickvalue{0U};
};