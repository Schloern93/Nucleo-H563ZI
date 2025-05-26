
#pragma once

#include "interface_adc_channel.hpp"

class AdcChannel : public Interface_AdcChannel {
public:
  AdcChannel(AdcInstance adcInstanceInit,
             AdcChannelConfig channelInit,
             AdcRank rankInit,
             AdcSamplingTime samplingTimeInit)
      : adcInstance(adcInstanceInit),
        channel(channelInit),
        rank(rankInit),
        samplingTime(samplingTimeInit) {
  }

  AdcInstance GetAdcInstance() const override {
    return adcInstance;
  }
  AdcChannelConfig GetChannel() const override {
    return channel;
  }
  AdcRank GetRank() const override {
    return rank;
  }
  AdcSamplingTime GetSamplingTime() const override {
    return samplingTime;
  }

  void SetChannelRawValue(uint32_t value) override {
    channelRawValue = value;
  }

private:
  AdcInstance adcInstance{};
  AdcChannelConfig channel{};
  AdcRank rank{};
  AdcSamplingTime samplingTime{};
  uint32_t channelRawValue{};
  // TODO: Each Channel can have one fileter
};
