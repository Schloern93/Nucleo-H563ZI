#pragma once

#include "thread.hpp"

#include "interface_sensor.hpp"

#include "timeout.hpp"

class Task2 : public cpp_freertos::Thread {
public:
  Task2(Interface_SensorData<Units::Celsius> &externalTempSensorInit,
        Interface_SensorData<Units::Celsius> &internalTempSensorInit)
      : cpp_freertos::Thread("Task2", 260U, (configMAX_PRIORITIES - 1U)),
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
      int64_t asdasdasdasd = data[1].value.getRawValue();
      // Units::DeciCelsius kjkjkjkjk = data[0].value.cast<Units::DeciCelsius>();

      // Units::Volt volt1{10};
      // Units::MilliVolt milliVolt2{200};
      // Units::Celsius celsius1{25};
      // Units::Kelvin kelvin1{500};

      // Units::MilliVolt v5 = volt1 + milliVolt2;
      // // Units::MilliVolt v6 = milliVolt2 - kelvin1;

      // Units::Celsius a1 = data[0].value + data[0].value;

      char buffer[160];
      vTaskGetRunTimeStats(buffer);
    }
  }

private:
  Interface_SensorData<Units::Celsius> &externalTempSensor;
  Interface_SensorData<Units::Celsius> &internalTempSensor;
  Timeout timer;
  SensorData<Units::Celsius> data[2]{};
};