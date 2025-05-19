/********************************************************************
 Copyright (c) Truma Geraetetechnik, Putzbrunn. All rights reserved.
*********************************************************************/

#pragma once

#include <cassert>

#include "stm32h5xx_hal_adc.h"
#include "stm32h5xx_hal_rcc.h"
#include "stm32h5xx_hal_dma.h"

namespace Truma_n {

class Adc {
public:
  Adc() {
    Init();
  }
  ~Adc() {
    HAL_ADC_Stop(&hadc1);
    HAL_ADC_DeInit(&hadc1);
  };

  void Init() {

    // DMA Configuration
    __HAL_RCC_GPDMA1_CLK_ENABLE();
    // DMA configuration
    hdma_adc1.Instance = GPDMA1_Channel0_NS;
    hdma_adc1.Init.Request = GPDMA1_REQUEST_ADC1;
    hdma_adc1.Init.BlkHWRequest = DMA_BREQ_BLOCK;
    hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc1.Init.SrcInc = DMA_SINC_FIXED;
    hdma_adc1.Init.DestInc = DMA_DINC_INCREMENTED;
    hdma_adc1.Init.SrcDataWidth = DMA_SRC_DATAWIDTH_HALFWORD;  // TODO: check if this is correct
    hdma_adc1.Init.DestDataWidth = DMA_SRC_DATAWIDTH_HALFWORD; // TODO: check if this is correct
    hdma_adc1.Init.Priority = DMA_HIGH_PRIORITY;
    // hdma_adc1.Init.SrcBurstLength = 1;
    // hdma_adc1.Init.DestBurstLength = 1;
    hdma_adc1.Init.TransferAllocatedPort = DMA_SRC_ALLOCATED_PORT0 | DMA_DEST_ALLOCATED_PORT1;
    hdma_adc1.Init.TransferEventMode = DMA_TCEM_REPEATED_BLOCK_TRANSFER;
    hdma_adc1.Init.Mode = DMA_PFCTRL;
    if(HAL_DMA_Init(&hdma_adc1) != HAL_OK) {
      assert(0); // ERROR HANDLING
    }
    // LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPDMA1 | LL_AHB1_GRP1_PERIPH_CRC | LL_AHB1_GRP1_PERIPH_SRAM1);
    HAL_NVIC_SetPriority(GPDMA1_Channel0_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(GPDMA1_Channel0_IRQn);

    __HAL_RCC_ADC_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    hadc1.Instance = ADC1;
    hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
    hadc1.Init.Resolution = ADC_RESOLUTION_12B;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
    hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;
    hadc1.Init.LowPowerAutoWait = DISABLE;
    hadc1.Init.ContinuousConvMode = ENABLE;
    hadc1.Init.NbrOfConversion = 2;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.NbrOfDiscConversion = 0;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc1.Init.SamplingMode = ADC_SAMPLING_MODE_NORMAL;
    hadc1.Init.DMAContinuousRequests = ENABLE;
    hadc1.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
    hadc1.Init.OversamplingMode = DISABLE;
    hadc1.Init.Oversampling = {0};
    hadc1.DMA_Handle = &hdma_adc1; // Set DMA Handle
    if(HAL_ADC_Init(&hadc1) != HAL_OK) {
      assert(0); // ERROR HANDLING
    }

    // Automatische Kalibrierung starten
    if(HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED) != HAL_OK) {
      assert(0);
    }

    // Channel configuration
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_247CYCLES_5;
    sConfig.SingleDiff = ADC_SINGLE_ENDED;
    sConfig.OffsetNumber = ADC_OFFSET_NONE;

    if(HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
      assert(0); // ERROR HANDLING
    }

    // Channel
    ADC_ChannelConfTypeDef sConfig1 = {0};
    sConfig1.Channel = ADC_CHANNEL_6;
    sConfig1.Rank = ADC_REGULAR_RANK_2;
    sConfig1.SamplingTime = ADC_SAMPLETIME_247CYCLES_5;
    sConfig1.SingleDiff = ADC_SINGLE_ENDED;
    sConfig1.OffsetNumber = ADC_OFFSET_NONE;

    if(HAL_ADC_ConfigChannel(&hadc1, &sConfig1) != HAL_OK) {
      assert(0); // ERROR HANDLING
    }
  }

  void StartADC() {
    if(HAL_ADC_Start_DMA(&hadc1, adcValues, 2)) {
      assert(0); // ERROR HANDLING
    }
  }

  void GetValue(uint32_t &value1, uint32_t &value2) {
    value1 = adcValues[0];
    uint32_t test = __HAL_ADC_CALC_TEMPERATURE(3300, value1, ADC_RESOLUTION_12B);
    value2 = adcValues[1];
    uint32_t test2 = __HAL_ADC_CALC_TEMPERATURE(3300, value2, ADC_RESOLUTION_12B);
  }

private:
  ADC_HandleTypeDef hadc1;
  DMA_HandleTypeDef hdma_adc1;
  uint32_t adcValues[2]{0};
};

} // namespace Truma_n

extern "C" void GPDMA1_Channel0_IRQHandler(void) {
  uint32_t status = GPDMA1_Channel0_NS->CSR; // Status-Register lesen

  if(status & DMA_FLAG_TC) {
    printf("Transfer Complete Interrupt\n");
    GPDMA1_Channel0_NS->CFCR = DMA_FLAG_TC; // Transfer Complete Flag löschen
  }

  if(status & DMA_FLAG_DTE) {
    printf("Data Transfer Error Interrupt\n");
    GPDMA1_Channel0_NS->CFCR = DMA_FLAG_DTE; // Fehler-Flag löschen
  }

  if(status & DMA_FLAG_HT) {
    printf("Half Transfer Complete Interrupt\n");
    GPDMA1_Channel0_NS->CFCR = DMA_FLAG_HT; // Half Transfer Complete Flag löschen
  }

  if(status & DMA_FLAG_SUSP) {
    printf("Suspension Complete Interrupt\n");
    GPDMA1_Channel0_NS->CFCR = DMA_FLAG_SUSP; // Suspension Flag löschen
  }
}