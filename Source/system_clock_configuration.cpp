#include "system_clock_configuration.hpp"

SystemClockConfiguration::SystemClockConfiguration() {
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PLL2InitTypeDef RCC_PLL2InitStruct = {0};

  // Configure the main internal regulator output voltage
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {
  }

  // Initializes the RCC Oscillators
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLL1_SOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 2;
  RCC_OscInitStruct.PLL.PLLN = 32;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1_VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1_VCORANGE_WIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    assert(false);
  }

  // Initializes the CPU, AHB and APB buses clocks
  RCC_ClkInitStruct.ClockType =
      RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_PCLK3;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;

  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
    assert(false);
  }

  // Configure the PLL2
  RCC_PLL2InitStruct.PLL2Source = RCC_PLL2_SOURCE_HSE;
  RCC_PLL2InitStruct.PLL2M = 2;
  RCC_PLL2InitStruct.PLL2N = 32;
  RCC_PLL2InitStruct.PLL2P = 2;
  RCC_PLL2InitStruct.PLL2Q = 2;
  RCC_PLL2InitStruct.PLL2R = 4;
  RCC_PLL2InitStruct.PLL2RGE = RCC_PLL2_VCIRANGE_3;
  RCC_PLL2InitStruct.PLL2VCOSEL = RCC_PLL2_VCORANGE_WIDE;
  RCC_PLL2InitStruct.PLL2FRACN = 0;
  RCC_PLL2InitStruct.PLL2ClockOut = RCC_PLL2_DIVP | RCC_PLL2_DIVQ | RCC_PLL2_DIVR;

  if(HAL_RCCEx_EnablePLL2(&RCC_PLL2InitStruct) != HAL_OK) {
    assert(false);
  }

  // Configure the programming delay
  __HAL_FLASH_SET_PROGRAM_DELAY(FLASH_PROGRAMMING_DELAY_2);
}
