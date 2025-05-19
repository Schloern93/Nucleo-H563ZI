
#pragma once

#include <cassert>

#include "stm32h5xx_hal.h"
#include "stm32h5xx_hal_adc.h"
#include "stm32h5xx_hal_cortex.h"
#include "stm32h5xx_hal_gpio.h"
#include "stm32h5xx_hal_rcc.h"

class Adc {
public:
  Adc() {
    InitGpio();
    InitAdc();
  }
  ~Adc() {
    HAL_ADC_Stop(&hadc1);
    HAL_ADC_DeInit(&hadc1);
  }

  void UpdateAdcValues() {
    if(HAL_ADC_Start(&hadc1) != HAL_OK) {
      assert(0);
    }

    for(int i = 0; i < 2; ++i) {
      if(HAL_ADC_PollForConversion(&hadc1, 1000) != HAL_OK) {
        assert(0);
      }
      adcData[i] = HAL_ADC_GetValue(&hadc1);
    }

    if(HAL_ADC_Stop(&hadc1) != HAL_OK) {
      assert(0); // ERROR HANDLING
    }
  }

  uint32_t *getData() {
    UpdateAdcValues();
    uint32_t externalTemp = __HAL_ADC_CALC_TEMPERATURE(3300, adcData[0], ADC_RESOLUTION_12B);
    uint32_t internalTemp = __HAL_ADC_CALC_TEMPERATURE(3300, adcData[1], ADC_RESOLUTION_12B);
    return adcData;
  }

  void InitGpio() {
    __HAL_RCC_GPIOF_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
  }

  void InitAdc() {
    // __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_ADC_CLK_ENABLE();
    LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_PATH_INTERNAL_TEMPSENSOR);

    hadc1.Instance = ADC1;
    hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV6;
    hadc1.Init.Resolution = ADC_RESOLUTION_12B;
    hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
    hadc1.Init.ContinuousConvMode = DISABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion = 2;
    hadc1.Init.DMAContinuousRequests = DISABLE;
    hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;

    hadc1.DMA_Handle = nullptr; // No DMA

    if(HAL_ADC_Init(&hadc1) != HAL_OK) {
      assert(0); // ERROR HANDLING
    }

    HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);

    // Kanal 6: PF12 – externer Sensor
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = ADC_CHANNEL_6;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_47CYCLES_5; // anpassen je nach Sensor
    sConfig.SingleDiff = ADC_SINGLE_ENDED;
    sConfig.OffsetNumber = ADC_OFFSET_NONE;
    sConfig.Offset = 0;
    if(HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
      assert(0); // ERROR HANDLING
    }

    // Kanal 16: interner Temp-Sensor
    sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
    sConfig.Rank = ADC_REGULAR_RANK_2;
    sConfig.SamplingTime = ADC_SAMPLETIME_247CYCLES_5; // Wichtig für TS
    if(HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
      assert(0); // ERROR HANDLING
    }
  }

private:
  ADC_HandleTypeDef hadc1{};
  ADC_HandleTypeDef hadc2{};
  uint32_t adcData[2]{};
};
