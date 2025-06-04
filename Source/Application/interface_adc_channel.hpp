
#pragma once

#include <cstdint>

enum class ReferenceVoltage {
  MV_3300,
  MV_5000,
};

enum class AdcSamplingTime {
  CYCLES_2_5,
  CYCLES_6_5,
  CYCLES_12_5,
  CYCLES_24_5,
  CYCLES_47_5,
  CYCLES_92_5,
  CYCLES_247_5,
  CYCLES_640_5,
};

enum AdcChannelConfig {
  CHANNEL_0,
  CHANNEL_1,
  CHANNEL_2,
  CHANNEL_3,
  CHANNEL_4,
  CHANNEL_5,
  CHANNEL_6,
  CHANNEL_7,
  CHANNEL_8,
  CHANNEL_9,
  CHANNEL_10,
  CHANNEL_11,
  CHANNEL_12,
  CHANNEL_13,
  CHANNEL_14,
  CHANNEL_15,
  CHANNEL_16,
  CHANNEL_17,
  CHANNEL_18,
  CHANNEL_19,
};

enum class AdcRank {
  RANK_1,
  RANK_2,
  RANK_3,
  RANK_4,
  RANK_5,
  RANK_6,
  RANK_7,
  RANK_8,
  RANK_9,
  RANK_10,
  RANK_11,
  RANK_12,
  RANK_13,
  RANK_14,
  RANK_15,
  RANK_16,
};

class Interface_AdcChannelConfig {
public:
  virtual ~Interface_AdcChannelConfig() = default;

  virtual AdcChannelConfig GetChannel() const = 0;
  virtual AdcRank GetRank() const = 0;
  virtual AdcSamplingTime GetSamplingTime() const = 0;
  virtual void SetAdcResolution(uint32_t resolution) = 0;

  virtual void SetChannelRawValue(uint32_t value) = 0;
};
