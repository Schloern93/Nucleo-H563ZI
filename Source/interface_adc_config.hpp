
#pragma once

enum class AdcInstance {
  ADC_1,
  ADC_2,
};

enum class AdcResolution {
  RESOLUTION_12_BIT,
  RESOLUTION_10_BIT,
  RESOLUTION_8_BIT,
  RESOLUTION_6_BIT,
};

enum class AdcReferenceVoltage {
  MV_3300,
  MV_5000,
};

class Interface_AdcConfig {
public:
  virtual ~Interface_AdcConfig() = default;

  virtual void UpdateAdcChannels() = 0;
};
