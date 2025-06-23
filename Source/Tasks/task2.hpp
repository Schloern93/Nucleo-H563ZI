#pragma once

#include "thread.hpp"

#include "interface_sensor.hpp"

#include "timeout.hpp"

class Task2 : public cpp_freertos::Thread {
public:
  Task2(Interface_SensorData &externalTempSensorInit, Interface_SensorData &internalTempSensorInit)
      : cpp_freertos::Thread("Task2", 200U, (configMAX_PRIORITIES - 1U)),
        externalTempSensor(externalTempSensorInit),
        internalTempSensor(internalTempSensorInit) {
    this->Start();
  }

  void Run() override {
    // Task implementation goes here
    while(true) {

      if(timer.IsExpired()) {
        timer.StartMs(3000U);
      }
      data[0] = externalTempSensor.GetSensorData();
      data[1] = internalTempSensor.GetSensorData();
      volatile uint32_t dummy = data[0].data + data[1].data; // Dummy operation to prevent optimization
    }
  }

private:
  Interface_SensorData &externalTempSensor;
  Interface_SensorData &internalTempSensor;
  Timeout timer;
  SensorData data[2]{};
};