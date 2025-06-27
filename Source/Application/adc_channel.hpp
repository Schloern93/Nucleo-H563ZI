
#pragma once

#include "interface_adc_channel.hpp"
#include "interface_sensor.hpp"

template <typename QuantityT>
class AdcChannel : public Interface_AdcChannelConfig, public Interface_SensorData<QuantityT> {
public:
  AdcChannel(const Interface_Sensor<QuantityT> &sensorInit,
             const AdcChannelConfig channelInit,
             const AdcRank rankInit,
             const AdcSamplingTime samplingTimeInit,
             const ReferenceVoltage referenceVoltageInit)
      : sensor(sensorInit),
        channel(channelInit),
        rank(rankInit),
        samplingTime(samplingTimeInit),
        referenceVoltage(SetReferenceVoltage(referenceVoltageInit)) {
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

  void SetChannelRawValue(uint32_t chRawValue) override {
    channelRawValue = chRawValue;
  }

  void SetAdcResolution(uint32_t resolution) override {
    if(!isResolutionSet) {
      adcResolution = resolution;
      isResolutionSet = true;
    }
  }

  SensorData<QuantityT> GetSensorData() const override {
    return sensor.CalculateSensorData(channelRawValue, referenceVoltage, adcResolution);
  }

private:
  const Interface_Sensor<QuantityT> &sensor;
  const AdcChannelConfig channel{};
  const AdcRank rank{};
  const AdcSamplingTime samplingTime{};
  const uint32_t referenceVoltage;
  uint32_t adcResolution{};
  bool isResolutionSet{false};
  uint32_t channelRawValue{0};

  static constexpr uint32_t SetReferenceVoltage(ReferenceVoltage referenceVoltage) {
    switch(referenceVoltage) {
    case ReferenceVoltage::MV_3300: return 3300U; break;
    case ReferenceVoltage::MV_5000: return 5000U; break;
    default: assert(false); return 0U;
    }
  }
};
