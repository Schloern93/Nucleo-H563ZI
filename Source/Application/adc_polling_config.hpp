
#pragma once

#include <array>

#include "stm32h5xx_hal.h"
#include "stm32h5xx_hal_adc.h"
#include "stm32h5xx_hal_adc_ex.h"
#include "stm32h5xx_hal_rcc.h"

#include "interface_adc_config.hpp"
#include "interface_adc_channel.hpp"

template <size_t NumberOfChannels> class AdcPollingConfig : public Interface_AdcConfig {
public:
  AdcPollingConfig(AdcInstance adcInstanceInit,
                   const AdcResolution adcResolutionInit,
                   std::array<Interface_AdcChannelConfig *, NumberOfChannels> adcChannelInit)
      : adcInstance(adcInstanceInit),
        adcResolution(adcResolutionInit),
        adcChannels(adcChannelInit) {
    // Set ADC Resolution to channels
    for(const auto &channel : adcChannels) {
      channel->SetAdcResolution(GetMaxValueForResolution(adcResolution));
    }
    InitAdcHandle();
    UpdateAdcChannels();
  }
  ~AdcPollingConfig() override {
    DeinitAdcHandle();
  }

  void UpdateAdcChannels() override {
    if(HAL_ADC_Start(&hadc) != HAL_OK) {
      assert(0);
    }
    for(size_t i = 0; i < NumberOfChannels; ++i) {
      if(HAL_ADC_PollForConversion(&hadc, ADC_POLL_TIMEOUT_MS) != HAL_OK) {
        assert(0);
      }
      uint32_t rawValue = HAL_ADC_GetValue(&hadc);
      adcChannels[i]->SetChannelRawValue(rawValue);
    }
    if(HAL_ADC_Stop(&hadc) != HAL_OK) {
      assert(0);
    }
  }

private:
  AdcInstance adcInstance;
  const AdcResolution adcResolution;
  std::array<Interface_AdcChannelConfig *, NumberOfChannels> adcChannels;
  ADC_HandleTypeDef hadc{};
  inline static uint8_t adcInstanceCounter{0};
  static constexpr uint16_t ADC_POLL_TIMEOUT_MS = 10;

  void InitAdcHandle() {
    __HAL_RCC_ADC_CLK_ENABLE();

    if(adcInstance == AdcInstance::ADC_1) {
      for(const auto &channel : adcChannels) {
        if(channel->GetChannel() == AdcChannelConfig::CHANNEL_16) {
          LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_PATH_INTERNAL_TEMPSENSOR);
        }
        if(channel->GetChannel() == AdcChannelConfig::CHANNEL_17) {
          LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_PATH_INTERNAL_VBAT);
        }
        if(channel->GetChannel() == AdcChannelConfig::CHANNEL_18) {
          LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_PATH_INTERNAL_VREFINT);
        }
      }
    }

    hadc.Instance = MapInstance(adcInstance);
    hadc.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV6;
    hadc.Init.Resolution = MapResolution(adcResolution);
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
    if(HAL_ADCEx_Calibration_Start(&hadc, ADC_SINGLE_ENDED) != HAL_OK) {
      assert(0);
    }
    InitAdcChannels();
    adcInstanceCounter++;
  }

  void DeinitAdcHandle() {
    if(HAL_ADC_DeInit(&hadc) != HAL_OK) {
      assert(0);
    }
    adcInstanceCounter--;
    DisableAdcClock();
  }

  void InitAdcChannels() {
    ADC_ChannelConfTypeDef sConfig{};
    for(const auto &adcChannel : adcChannels) {
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

  void DisableAdcClock() {
    if(adcInstanceCounter == 0) {
      __HAL_RCC_ADC_CLK_DISABLE();
    }
  }

  constexpr ADC_TypeDef *MapInstance(AdcInstance instance) {
    switch(instance) {
    case AdcInstance::ADC_1: return ADC1;
    case AdcInstance::ADC_2: return ADC2;
    default: assert(false); return nullptr;
    }
  }

  static constexpr uint32_t MapResolution(AdcResolution resolution) {
    switch(resolution) {
    case AdcResolution::RESOLUTION_12_BIT: return ADC_RESOLUTION_12B;
    case AdcResolution::RESOLUTION_10_BIT: return ADC_RESOLUTION_10B;
    case AdcResolution::RESOLUTION_8_BIT: return ADC_RESOLUTION_8B;
    case AdcResolution::RESOLUTION_6_BIT: return ADC_RESOLUTION_6B;
    default: assert(false); return 0;
    }
  }

  static constexpr uint32_t MapChannel(AdcChannelConfig channel) {
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

  static constexpr uint32_t MapRank(AdcRank rank) {
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

  static constexpr uint32_t MapSamplingTime(AdcSamplingTime samplingTime) {
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

  static constexpr uint16_t GetMaxValueForResolution(AdcResolution res) {
    switch(res) {
    case AdcResolution::RESOLUTION_12_BIT: return (1 << 12) - 1;
    case AdcResolution::RESOLUTION_10_BIT: return (1 << 10) - 1;
    case AdcResolution::RESOLUTION_8_BIT: return (1 << 8) - 1;
    case AdcResolution::RESOLUTION_6_BIT: return (1 << 6) - 1;
    default: assert(false); return 0;
    }
  }
};