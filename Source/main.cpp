#include <cassert>
#include <array>

//
#include "stm32h5xx_hal_gpio.h"

#include "thread.hpp"

// Applications Taks
#include "task1.hpp"
#include "task2.hpp"

// ADC configuration
#include "adc_polling_config.hpp"
#include "adc_channel.hpp"
#include "interface_adc_channel.hpp"
#include "mcu_temperature_sensor.hpp"

// Define ADC channels
static constexpr size_t ADC_CHANNEL_COUNT = 2;
AdcChannel adc0(AdcChannelConfig::CHANNEL_6, AdcRank::RANK_1, AdcSamplingTime::CYCLES_247_5);
AdcChannel adc1(AdcChannelConfig::CHANNEL_16, AdcRank::RANK_2, AdcSamplingTime::CYCLES_247_5);
std::array<Interface_AdcChannel *, ADC_CHANNEL_COUNT> adcChannels = {&adc0, &adc1};

// Create ADC
AdcPollingConfig<ADC_CHANNEL_COUNT>
    adc1PollingConfig(AdcInstance::ADC_1, AdcResolution::RESOLUTION_12_BIT, AdcReferenceVoltage::MV_3300, adcChannels);

// Create Sensors
McuTemperatureSensor externalTempSensor(adc0);
McuTemperatureSensor internalTempSensor(adc1); // Internal MCU temperature sensor

// Create tasks
Task1 task1(adc1PollingConfig);
Task2 task2(externalTempSensor, internalTempSensor);

int main() {
  __HAL_RCC_GPIOF_CLK_ENABLE();
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

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

size_t __write(size_t handle, const void *buffer, size_t size) {
  assert(false);
  return size;
}
}
