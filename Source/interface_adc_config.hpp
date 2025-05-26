
#pragma once

enum class AdcInstance {
  ADC_1,
  ADC_2,
};

enum class AdcClockPrescaler {
  ASYNC_DIV1,
  ASYNC_DIV2,
  ASYNC_DIV4,
  ASYNC_DIV6,
  ASYNC_DIV8,
  ASYNC_DIV10,
  ASYNC_DIV12,
  ASYNC_DIV16,
};

class Interface_AdcConfig {
public:
  virtual ~Interface_AdcConfig() = default;

  virtual AdcInstance GetAdcInstance() const = 0;

  virtual uint32_t GetClockPrescaler() const = 0;
  virtual uint32_t GetResolution() const = 0;
  virtual uint32_t GetScanConvMode() const = 0;
  virtual uint32_t GetContinuousConvMode() const = 0;
  virtual uint32_t GetDiscontinuousConvMode() const = 0;
  virtual uint32_t GetExternalTrigConv() const = 0;
  virtual uint32_t GetDataAlign() const = 0;
  virtual uint32_t GetNbrOfConversion() const = 0;
  virtual uint32_t GetDMAContinuousRequests() const = 0;
  virtual uint32_t GetEocSelection() const = 0;
};
