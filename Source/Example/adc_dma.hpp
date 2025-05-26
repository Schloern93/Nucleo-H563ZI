
#pragma once

#include <cassert>

#include "stm32h5xx_hal_gpio.h"
#include "stm32h5xx_hal_rcc.h"
#include "stm32h5xx_hal_dma.h"
#include "stm32h5xx_hal_dma_ex.h"
#include "stm32h5xx_hal_adc.h"
#include "stm32h5xx_hal_adc_ex.h"
#include "stm32h5xx_hal_cortex.h"

class ADC_Test {
public:
  ADC_Test() {
    InitGpio();
    // InitDma();
    InitDmaLinkedList();
    InitAdc();
  }
  ~ADC_Test() {
    // Destructor
  }

  void StartAdcWithDma() {
    if(HAL_ADC_Start_DMA(&hadc1, adcData, 2) != HAL_OK) {
      assert(0);
    }
  }

  uint32_t *getData() {
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

    hadc1.DMA_Handle = &hdma_adc1; // USE HAL FUnction

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

  // void InitDma() {
  //   __HAL_RCC_GPDMA1_CLK_ENABLE();
  //   // HAL_NVIC_SetPriority(GPDMA1_Channel1_IRQn, 0, 0);
  //   // HAL_NVIC_EnableIRQ(GPDMA1_Channel1_IRQn);

  //   hdma_adc1.Instance = GPDMA1_Channel1_NS;
  //   hdma_adc1.Init.Request = GPDMA1_REQUEST_ADC1;
  //   hdma_adc1.Init.BlkHWRequest = DMA_BREQ_SINGLE_BURST;
  //   hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
  //   hdma_adc1.Init.SrcInc = DMA_SINC_FIXED;
  //   hdma_adc1.Init.DestInc = DMA_DINC_INCREMENTED;
  //   hdma_adc1.Init.SrcDataWidth = DMA_SRC_DATAWIDTH_WORD;
  //   hdma_adc1.Init.DestDataWidth = DMA_DEST_DATAWIDTH_WORD;
  //   hdma_adc1.Init.Priority = DMA_LOW_PRIORITY_LOW_WEIGHT;
  //   hdma_adc1.Init.SrcBurstLength = 1;
  //   hdma_adc1.Init.DestBurstLength = 1;
  //   hdma_adc1.Init.TransferAllocatedPort = DMA_SRC_ALLOCATED_PORT0 | DMA_DEST_ALLOCATED_PORT1;
  //   hdma_adc1.Init.TransferEventMode = DMA_TCEM_BLOCK_TRANSFER;
  //   hdma_adc1.Init.Mode = DMA_NORMAL;

  //   if(HAL_DMA_Init(&hdma_adc1) != HAL_OK) {
  //     assert(0); // Fehlerbehandlung
  //   }
  // }

  void InitDmaLinkedList() {
    __HAL_RCC_GPDMA1_CLK_ENABLE();
    // HAL_NVIC_SetPriority(GPDMA1_Channel1_IRQn, 0, 0);
    // HAL_NVIC_EnableIRQ(GPDMA1_Channel1_IRQn);

    HAL_DMAEx_List_ResetQ(&dmaList);

    // Node-Konfiguration identisch zur Handle-Init
    node_conf_adc1.NodeType = DMA_GPDMA_LINEAR_NODE;
    node_conf_adc1.Init.Request = GPDMA1_REQUEST_ADC1;
    node_conf_adc1.Init.BlkHWRequest = DMA_BREQ_SINGLE_BURST;
    node_conf_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
    node_conf_adc1.Init.SrcInc = DMA_SINC_FIXED;
    node_conf_adc1.Init.DestInc = DMA_DINC_INCREMENTED;
    node_conf_adc1.Init.SrcDataWidth = DMA_SRC_DATAWIDTH_WORD;
    node_conf_adc1.Init.DestDataWidth = DMA_DEST_DATAWIDTH_WORD;
    node_conf_adc1.Init.Priority = DMA_LOW_PRIORITY_LOW_WEIGHT;
    node_conf_adc1.Init.SrcBurstLength = 1;
    node_conf_adc1.Init.DestBurstLength = 1;
    node_conf_adc1.Init.TransferAllocatedPort = DMA_SRC_ALLOCATED_PORT0 | DMA_DEST_ALLOCATED_PORT1;
    node_conf_adc1.Init.TransferEventMode = DMA_TCEM_BLOCK_TRANSFER;
    node_conf_adc1.Init.Mode = DMA_NORMAL;

    node_conf_adc1.SrcAddress = (uint32_t)&ADC1->DR;
    node_conf_adc1.DstAddress = (uint32_t)adcData;
    node_conf_adc1.DataSize = sizeof(adcData);

    // Linked List aufbauen
    if(HAL_DMAEx_List_BuildNode(&node_conf_adc1, &dmaNode) != HAL_OK) {
      assert(0); // Fehler beim Erstellen des Knotens
    }
    if(HAL_DMAEx_List_InsertNode_Tail(&dmaList, &dmaNode) != HAL_OK) {
      assert(0); // Fehler beim Einfügen des Knotens in die Liste
    }
    if(HAL_DMAEx_List_SetCircularMode(&dmaList) != HAL_OK) {
      assert(0); // Fehler beim Setzen des zirkularen Modus
    }
    if(HAL_DMAEx_List_ConvertQToDynamic(&dmaList) != HAL_OK) {
      assert(0); // Fehler beim Konvertieren der Liste in den dynamischen Modus
    }

    // Handle vorbereiten
    hdma_adc1.Instance = GPDMA1_Channel1_NS;
    hdma_adc1.InitLinkedList.Priority = DMA_LOW_PRIORITY_LOW_WEIGHT;
    hdma_adc1.InitLinkedList.LinkStepMode = DMA_LSM_FULL_EXECUTION;
    hdma_adc1.InitLinkedList.LinkAllocatedPort = DMA_SRC_ALLOCATED_PORT0 | DMA_DEST_ALLOCATED_PORT1;
    hdma_adc1.InitLinkedList.TransferEventMode = DMA_TCEM_BLOCK_TRANSFER;
    hdma_adc1.InitLinkedList.LinkedListMode = DMA_LINKEDLIST_CIRCULAR;

    if(HAL_DMAEx_List_Init(&hdma_adc1) != HAL_OK) {
      assert(0); // Fehlerbehandlung
    }

    if(HAL_DMA_ConfigChannelAttributes(&hdma_adc1, DMA_CHANNEL_NSEC) != HAL_OK) {
      assert(0); // Fehlerbehandlung
    }

    // Verlinke die Liste mit dem DMA-Handle
    if(HAL_DMAEx_List_LinkQ(&hdma_adc1, &dmaList) != HAL_OK) {
      assert(0); // Fehler beim Verlinken der Liste mit dem Handle
    }
  }

private:
  ADC_HandleTypeDef hadc1{};
  DMA_HandleTypeDef hdma_adc1{};
  DMA_NodeTypeDef dmaNode{};
  DMA_NodeConfTypeDef node_conf_adc1{};
  DMA_QListTypeDef dmaList{};
  uint32_t adcData[2]{};
};

// // Needet without linked list config to start the conversion after a finished transfer
// extern "C" void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
//   if(hadc->Instance == ADC1) {
//     HAL_ADC_Start_DMA(hadc, adcData, 2);
//   }
// }