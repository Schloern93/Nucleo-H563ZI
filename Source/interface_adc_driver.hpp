
#pragma once

#include <cstdint>

struct AdcConfigChannel {
  uint32_t channel;
  uint32_t rank;
  uint32_t samplingTime;
};

enum class AdcInstance {
  ADC_1,
  ADC_2,
};

struct AdcConfig {
  AdcInstance instance;
  uint32_t clockPrescaler;
  uint32_t resolution;
  uint32_t scanConvMode;
  uint32_t continuousConvMode;
  uint32_t discontinuousConvMode;

  uint32_t lowPowerAutoWait;
  uint32_t lowPowerFrequencyMode;
  uint32_t numberOfConversion;
  AdcConfigChannel *channels;
};

class Interface_AdcDriver {
public:
  virtual ~Interface_AdcDriver() = default;
  virtual void GetAdcRawData(uint32_t *data, size_t len) = 0;
};