
#pragma once

#include "interface_adc_channel.hpp"

class AdcChannel : public Interface_AdcChannel, public Interface_AdcChannelGetter {
public:
  AdcChannel(AdcChannelConfig channelInit, AdcRank rankInit, AdcSamplingTime samplingTimeInit)
      : channel(channelInit),
        rank(rankInit),
        samplingTime(samplingTimeInit) {
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

  void SetChannelVoltageMv(uint32_t voltageMv) override {
    channelVoltageMv = voltageMv;
  }

  uint32_t GetVoltageMv() const override {
    return channelVoltageMv;
  }

private:
  AdcChannelConfig channel{};
  AdcRank rank{};
  AdcSamplingTime samplingTime{};
  uint32_t channelVoltageMv{};
};
