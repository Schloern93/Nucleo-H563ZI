
#pragma once

#include <cassert>

#include "stm32h5xx_hal_gpio.h"
#include "stm32h5xx_hal_rcc.h"
#include "stm32h5xx_hal_dma.h"
#include "stm32h5xx_hal_dma_ex.h"
#include "stm32h5xx_hal_adc.h"
#include "stm32h5xx_hal_cortex.h"

class ADC_Test {
public:
  ADC_Test() {
    InitGpio();
    InitDma();
    InitAdc();
  }
  ~ADC_Test() {
    // Destructor
  }

  void start() {
    if(HAL_ADC_Start_DMA(&hadc1, adcData, 2) != HAL_OK) {
      assert(0);
    }
  }

  uint32_t *getData() {
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
    __HAL_RCC_ADC_CLK_ENABLE();
    LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_PATH_INTERNAL_TEMPSENSOR);

    hadc1.Instance = ADC1;
    hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
    hadc1.Init.Resolution = ADC_RESOLUTION_12B;
    hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE; // Mehrere Kanäle!
    hadc1.Init.ContinuousConvMode = ENABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion = 2;
    hadc1.Init.DMAContinuousRequests = ENABLE;
    hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;

    hadc1.DMA_Handle = &hdma_adc1;

    if(HAL_ADC_Init(&hadc1) != HAL_OK) {
      assert(0); // ERROR HANDLING
    }

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

  void InitDma() {
    __HAL_RCC_GPDMA1_CLK_ENABLE();
    // HAL_NVIC_SetPriority(GPDMA1_Channel1_IRQn, 0, 0);
    // HAL_NVIC_EnableIRQ(GPDMA1_Channel1_IRQn);

    hdma_adc1.Instance = GPDMA1_Channel1_NS;
    hdma_adc1.Init.Request = GPDMA1_REQUEST_ADC1;
    hdma_adc1.Init.BlkHWRequest = DMA_BREQ_SINGLE_BURST;
    hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc1.Init.SrcInc = DMA_SINC_FIXED;
    hdma_adc1.Init.DestInc = DMA_DINC_INCREMENTED;
    hdma_adc1.Init.SrcDataWidth = DMA_SRC_DATAWIDTH_WORD;
    hdma_adc1.Init.DestDataWidth = DMA_DEST_DATAWIDTH_WORD;
    hdma_adc1.Init.Priority = DMA_LOW_PRIORITY_LOW_WEIGHT;
    hdma_adc1.Init.SrcBurstLength = 1;
    hdma_adc1.Init.DestBurstLength = 1;
    hdma_adc1.Init.TransferAllocatedPort = DMA_SRC_ALLOCATED_PORT0 | DMA_DEST_ALLOCATED_PORT1;
    hdma_adc1.Init.TransferEventMode = DMA_TCEM_BLOCK_TRANSFER;
    hdma_adc1.Init.Mode = DMA_NORMAL;

    if(HAL_DMA_Init(&hdma_adc1) != HAL_OK) {
      assert(0); // Fehlerbehandlung
    }
  }

private:
  ADC_HandleTypeDef hadc1{};
  DMA_HandleTypeDef hdma_adc1{};
  uint32_t adcData[2]{};
};