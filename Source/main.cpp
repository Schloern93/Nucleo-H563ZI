#include <cassert>
#include <array>

// Hal
#include "stm32h5xx_hal_gpio.h"

// Clocks
#include "system_clock_configuration.hpp"

// Rtos
#include "thread.hpp"

// Applications Taks
#include "task1.hpp"
#include "task2.hpp"

// ADC configuration
#include "adc_polling_config.hpp"
#include "adc_channel.hpp"
#include "interface_adc_channel.hpp"
#include "thermistor_sensor.hpp"
#include "mcu_temp_sensor.hpp"

// Create System Clock
SystemClockConfiguration systemClockConfiguration;

// Create Sensors
ThermistorSensor<10000, 30, semitec103ATCurve> externalTempSensor;
McuTempSensor internalTempSensor;

// Define ADC channels
static constexpr size_t ADC_CHANNEL_COUNT = 2;
AdcChannel externalTemperaturSensor(externalTempSensor,
                                    AdcChannelConfig::CHANNEL_6,
                                    AdcRank::RANK_1,
                                    AdcSamplingTime::CYCLES_247_5,
                                    ReferenceVoltage::MV_3300);
AdcChannel mcuTempSensor(internalTempSensor,
                         AdcChannelConfig::CHANNEL_16,
                         AdcRank::RANK_2,
                         AdcSamplingTime::CYCLES_247_5,
                         ReferenceVoltage::MV_3300);
std::array<Interface_AdcChannelConfig *, ADC_CHANNEL_COUNT> adcChannels = {&externalTemperaturSensor, &mcuTempSensor};

// Create ADC
AdcPollingConfig<ADC_CHANNEL_COUNT> adc1PollingConfig(AdcInstance::ADC_1,
                                                      AdcResolution::RESOLUTION_12_BIT,
                                                      adcChannels);

// Create tasks
Task1 task1(adc1PollingConfig);
Task2 task2(externalTemperaturSensor, mcuTempSensor);

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

// Needet for zhe IAR stdLib
size_t __write(size_t handle, const void *buffer, size_t size) {
  assert(false);
  return size;
}
}
