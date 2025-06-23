/********************************************************************
Copyright (c) Truma Geraetetechnik, Putzbrunn. All rights reserved.
********************************************************************/
#pragma once

#include <cstdint>

/// \brief Interface for a timeout class
class Interface_Timeout {
public:
  /// \brief Start a timeout
  /// \param new_timeout time for the timeout
  virtual void StartMs(const std::uint32_t new_timeout) = 0;

  /// \brief Checks if a timeout is expired
  /// \return True if the timeout has expired (Elapsed time > timeout) or timeout set immediatly (timeout == 0U).
  virtual bool IsExpired() const = 0;

  /// @brief  \brief Returns the time elapsed since last call to StartMs
  /// @return elapsed time in milliseconds
  virtual uint32_t ElapsedMs() const = 0;
};
