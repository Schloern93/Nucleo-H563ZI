
#pragma once

#include <array>

#include "stm32h5xx_hal.h"
#include "stm32h5xx_hal_adc.h"
#include "stm32h5xx_hal_adc_ex.h"
#include "stm32h5xx_hal_cortex.h"

#include "stm32h5xx_hal_rcc.h"

#include "interface_adc_config.hpp"
#include "interface_adc_channel.hpp"

template <size_t NumberOfChannles> class AdcPollingConfig : public Interface_AdcConfig {
public:
  AdcPollingConfig(AdcInstance adcInstanceInit, std::array<Interface_AdcChannel *, NumberOfChannles> adcChannelInit)
      : adcInstance(adcInstanceInit),
        adcChannels(adcChannelInit) {
  }

  void Init() override {
    LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_PATH_INTERNAL_TEMPSENSOR);
    switch(adcInstance) {
    case AdcInstance::ADC_1: hadc.Instance = ADC1; break;
    case AdcInstance::ADC_2: hadc.Instance = ADC2; break;
    default: assert(0); return;
    }
    InitAdcHandle();
    InitAdcChannels();
  }

  void Deinit() override {
    DeinitAdcHandle();
  }

  void UpdateAdcValues() override {
    if(HAL_ADC_Start(&hadc) != HAL_OK) {
      assert(0); // Error handling
    }
    for(size_t i = 0; i < NumberOfChannles; ++i) {
      HAL_ADC_PollForConversion(&hadc, 1000);
      adcChannels[i]->SetChannelRawValue(HAL_ADC_GetValue(&hadc));
    }
    if(HAL_ADC_Stop(&hadc) != HAL_OK) {
      assert(0); // Error handling
    }
  }

private:
  AdcInstance adcInstance;
  std::array<Interface_AdcChannel *, NumberOfChannles> adcChannels; // TODO: Check uneque pointer
  ADC_HandleTypeDef hadc{};

  void InitAdcHandle() {
    __HAL_RCC_ADC_CLK_ENABLE();
    hadc.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV6; // TODO: Check for differnt clocks
    hadc.Init.Resolution = ADC_RESOLUTION_12B;
    hadc.Init.ScanConvMode = ADC_SCAN_ENABLE;
    hadc.Init.ContinuousConvMode = DISABLE;
    hadc.Init.DiscontinuousConvMode = DISABLE;
    hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc.Init.NbrOfConversion = adcChannels.size();
    hadc.Init.DMAContinuousRequests = DISABLE;
    hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;

    hadc.DMA_Handle = nullptr; // No DMA used

    if(HAL_ADC_Init(&hadc) != HAL_OK) {
      assert(0);
    }
    HAL_ADCEx_Calibration_Start(&hadc, ADC_SINGLE_ENDED);
  }

  void DeinitAdcHandle() {
    if(HAL_ADC_DeInit(&hadc) != HAL_OK) {
      assert(0);
    }
    __HAL_RCC_ADC_CLK_DISABLE();
  }

  void InitAdcChannels() {
    ADC_ChannelConfTypeDef sConfig{};
    for(auto &adcChannel : adcChannels) {
      sConfig.Channel = MapChannel(adcChannel->GetChannel());
      sConfig.Rank = MapRank(adcChannel->GetRank());
      sConfig.SamplingTime = MapSamplingTime(adcChannel->GetSamplingTime());
      sConfig.SingleDiff = ADC_SINGLE_ENDED;
      sConfig.OffsetNumber = ADC_OFFSET_NONE;
      if(HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK) {
        assert(0);
      }
    }
  }

  constexpr uint32_t MapChannel(AdcChannelConfig channel) {
    switch(channel) {
    case AdcChannelConfig::CHANNEL_0: return ADC_CHANNEL_0;
    case AdcChannelConfig::CHANNEL_1: return ADC_CHANNEL_1;
    case AdcChannelConfig::CHANNEL_2: return ADC_CHANNEL_2;
    case AdcChannelConfig::CHANNEL_3: return ADC_CHANNEL_3;
    case AdcChannelConfig::CHANNEL_4: return ADC_CHANNEL_4;
    case AdcChannelConfig::CHANNEL_5: return ADC_CHANNEL_5;
    case AdcChannelConfig::CHANNEL_6: return ADC_CHANNEL_6;
    case AdcChannelConfig::CHANNEL_7: return ADC_CHANNEL_7;
    case AdcChannelConfig::CHANNEL_8: return ADC_CHANNEL_8;
    case AdcChannelConfig::CHANNEL_9: return ADC_CHANNEL_9;
    case AdcChannelConfig::CHANNEL_10: return ADC_CHANNEL_10;
    case AdcChannelConfig::CHANNEL_11: return ADC_CHANNEL_11;
    case AdcChannelConfig::CHANNEL_12: return ADC_CHANNEL_12;
    case AdcChannelConfig::CHANNEL_13: return ADC_CHANNEL_13;
    case AdcChannelConfig::CHANNEL_14: return ADC_CHANNEL_14;
    case AdcChannelConfig::CHANNEL_15: return ADC_CHANNEL_15;
    case AdcChannelConfig::CHANNEL_16: return ADC_CHANNEL_16; // Internal temperature sensor
    case AdcChannelConfig::CHANNEL_17: return ADC_CHANNEL_17; // Internal Vbat
    case AdcChannelConfig::CHANNEL_18: return ADC_CHANNEL_18; // Internal VrefInt
    case AdcChannelConfig::CHANNEL_19: return ADC_CHANNEL_19;
    default: assert(false); return 0;
    }
  }

  constexpr uint32_t MapRank(AdcRank rank) {
    switch(rank) {
    case AdcRank::RANK_1: return ADC_REGULAR_RANK_1;
    case AdcRank::RANK_2: return ADC_REGULAR_RANK_2;
    case AdcRank::RANK_3: return ADC_REGULAR_RANK_3;
    case AdcRank::RANK_4: return ADC_REGULAR_RANK_4;
    case AdcRank::RANK_5: return ADC_REGULAR_RANK_5;
    case AdcRank::RANK_6: return ADC_REGULAR_RANK_6;
    case AdcRank::RANK_7: return ADC_REGULAR_RANK_7;
    case AdcRank::RANK_8: return ADC_REGULAR_RANK_8;
    case AdcRank::RANK_9: return ADC_REGULAR_RANK_9;
    case AdcRank::RANK_10: return ADC_REGULAR_RANK_10;
    case AdcRank::RANK_11: return ADC_REGULAR_RANK_11;
    case AdcRank::RANK_12: return ADC_REGULAR_RANK_12;
    case AdcRank::RANK_13: return ADC_REGULAR_RANK_13;
    case AdcRank::RANK_14: return ADC_REGULAR_RANK_14;
    case AdcRank::RANK_15: return ADC_REGULAR_RANK_15;
    case AdcRank::RANK_16: return ADC_REGULAR_RANK_16;
    default: assert(false); return 0;
    }
  }

  constexpr uint32_t MapSamplingTime(AdcSamplingTime samplingTime) {
    switch(samplingTime) {
    case AdcSamplingTime::CYCLES_2_5: return ADC_SAMPLETIME_2CYCLES_5;
    case AdcSamplingTime::CYCLES_6_5: return ADC_SAMPLETIME_6CYCLES_5;
    case AdcSamplingTime::CYCLES_12_5: return ADC_SAMPLETIME_12CYCLES_5;
    case AdcSamplingTime::CYCLES_24_5: return ADC_SAMPLETIME_24CYCLES_5;
    case AdcSamplingTime::CYCLES_47_5: return ADC_SAMPLETIME_47CYCLES_5;
    case AdcSamplingTime::CYCLES_92_5: return ADC_SAMPLETIME_92CYCLES_5;
    case AdcSamplingTime::CYCLES_247_5: return ADC_SAMPLETIME_247CYCLES_5;
    case AdcSamplingTime::CYCLES_640_5: return ADC_SAMPLETIME_640CYCLES_5;
    default: assert(false); return 0;
    }
  }
};