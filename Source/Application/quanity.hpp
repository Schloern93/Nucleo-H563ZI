#pragma once
#include <cstdint>
#include <ratio>
#include <type_traits>

template <typename UnitTag,                 // z. B. Celsius
          typename Rep = int32_t,           // zugrundeliegender Integer
          typename Scale = std::ratio<1, 1> // phys. Einheit : 1 Digit
          >
class Quantity {
public:
  using rep = Rep;
  using unit = UnitTag;
  using scale = Scale;

  // Rohwert-Konstruktor (z. B. 253  ⇒  25,3 °C bei Scale 1/10)
  constexpr explicit Quantity(rep raw = 0)
      : raw_{raw} {
  }

  constexpr rep raw() const {
    return raw_;
  }

  constexpr Quantity operator+(Quantity rhs) const {
    return Quantity{raw_ + rhs.raw_}; // TODO: Check here same sacle as well?
  }
  constexpr Quantity operator-(Quantity rhs) const {
    return Quantity{raw_ - rhs.raw_}; // TODO: Check here same sacle as well?
  }

  template <typename OutQ> constexpr OutQ cast() const {
    static_assert(std::is_same<typename OutQ::unit, unit>::value, "UnitTags are not same!");
    using ratio = std::ratio_divide<scale, typename OutQ::scale>;
    using big = std::intmax_t;

    big tmp = static_cast<big>(raw_) * ratio::num;
    tmp /= ratio::den;
    return OutQ{static_cast<typename OutQ::rep>(tmp)};
  }

private:
  rep raw_;
};
