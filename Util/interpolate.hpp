
#pragma once

#include <cstdint>

int32_t Interpolate(int32_t x, int32_t x0, int32_t x1, int32_t y0, int32_t y1) {
  if(x1 == x0) {
    return y0; // Avoid division by zero
  }
  return y0 + ((x - x0) * (y1 - y0)) / (x1 - x0);
}