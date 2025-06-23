
#pragma once

#include "stm32h5xx_hal_tim.h"

extern "C" {

TIM_HandleTypeDef htim2;

void ConfigureTim2ForFreeRtosRunTimeStatsTimer() {

  // Enable the clock for TIM2
  __HAL_RCC_TIM2_CLK_ENABLE();

  // Configure TIM2
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 24999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 0xFFFFFFFF;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if(HAL_TIM_Base_Init(&htim2) != HAL_OK) {
    assert(false);
  }

  // Start the timer
  if(HAL_TIM_Base_Start(&htim2) != HAL_OK) {
    assert(false);
  }
}

uint32_t GetFreeRtosRunTimeStatsTimerValue(void) {
  return __HAL_TIM_GET_COUNTER(&htim2);
}
}