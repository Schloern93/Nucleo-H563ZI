#pragma once

#include "thread.hpp"
#include "FreeRTOSConfig.h"

class Task1 : public cpp_freertos::Thread {
public:
  Task1()
      : cpp_freertos::Thread("Task1", 50U, (configMAX_PRIORITIES - 1U)) {
  }

  void Run() override {
    // Task implementation goes here
    while(true) {
      // Do something
      Delay(1000); // Delay for 1 second
    }
  }
};