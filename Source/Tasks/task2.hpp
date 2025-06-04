#pragma once

#include "thread.hpp"
#include "FreeRTOSConfig.h"

#include "interface_sensor.hpp"

class Task2 : public cpp_freertos::Thread {
public:
  Task2(Interface_SensorData &externalTempSensorInit, Interface_SensorData &internalTempSensorInit)
      : cpp_freertos::Thread("Task2", 100U, (configMAX_PRIORITIES - 1U)),
        externalTempSensor(externalTempSensorInit),
        internalTempSensor(internalTempSensorInit) {
    this->Start();
  }

  void Run() override {
    // Task implementation goes here
    while(true) {
      // Do something
      // Delay(1000); // Delay for 1 second
      data[0] = externalTempSensor.GetSensorData();
      data[1] = internalTempSensor.GetSensorData();
    }
  }

private:
  Interface_SensorData &externalTempSensor;
  Interface_SensorData &internalTempSensor;
  SensorData data[2]{};
};