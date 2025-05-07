#include <cassert>
#include "thread.hpp"

#include "task1.hpp"
#include "task2.hpp"

Task1 task1;
Task2 task2;

int main() {

  cpp_freertos::Thread::StartScheduler();

  while(true) {
  }
  return 0;
}

extern "C" {
void HardFault_Handler(void) {
  while(1) {
    assert(false);
  }
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
  // Handle stack overflow in freeRTOS
  while(1) {
    assert(false);
  }
}
}
