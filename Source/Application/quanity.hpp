#pragma once

#include <cassert>
#include <cstdint>
#include <ratio>
#include <type_traits>

template <typename UnitTag, typename Rep = int32_t, typename Scale = std::ratio<1, 1>> class Quantity {
public:
  using rep = Rep;
  using unit = UnitTag;
  using scale = Scale;

  constexpr explicit Quantity(rep rawInit = 0)
      : raw{rawInit} {
  }

  constexpr rep getRawValue() const {
    return raw;
  }

  template <typename OutQ> constexpr OutQ cast() const {
    static_assert(std::is_same<typename OutQ::unit, unit>::value, "UnitTags are not same!");
    using ratio = std::ratio_divide<scale, typename OutQ::scale>;
    using big = std::intmax_t;

    big tmp = static_cast<big>(raw) * ratio::num;
    tmp /= ratio::den;

    constexpr big minValue = std::numeric_limits<typename OutQ::rep>::min();
    constexpr big maxValue = std::numeric_limits<typename OutQ::rep>::max();
    if(tmp < minValue || tmp > maxValue) {
      assert(false && "Value out of range for target representation type");
    }
    return OutQ{static_cast<typename OutQ::rep>(tmp)};
  }

private:
  rep raw;
};

template <typename UnitTag, typename Rep1, typename Scale1, typename Rep2, typename Scale2>
constexpr auto operator+(const Quantity<UnitTag, Rep1, Scale1> &lhs, const Quantity<UnitTag, Rep2, Scale2> &rhs) {
  static_assert(std::is_same<UnitTag, typename Quantity<UnitTag, Rep2, Scale2>::unit>::value,
                "UnitTags are not the same!");

  using big = std::intmax_t;
  using targetScale =
      typename std::conditional<(Scale1::num * Scale2::den < Scale2::num * Scale1::den), Scale1, Scale2>::type;
  big result;

  if constexpr(std::is_same<Scale1, Scale2>::value) {
    result = static_cast<big>(lhs.getRawValue()) + static_cast<big>(rhs.getRawValue());

    constexpr big minValue = std::numeric_limits<Rep1>::min();
    constexpr big maxValue = std::numeric_limits<Rep1>::max();
    if(result < minValue || result > maxValue) {
      assert(false && "Result out of range for Rep type");
    }
  } else {
    using lhs_ratio = std::ratio_divide<Scale1, targetScale>;
    using rhs_ratio = std::ratio_divide<Scale2, targetScale>;

    big lhs_adjusted = static_cast<big>(lhs.getRawValue()) * lhs_ratio::num / lhs_ratio::den;
    big rhs_adjusted = static_cast<big>(rhs.getRawValue()) * rhs_ratio::num / rhs_ratio::den;

    result = lhs_adjusted + rhs_adjusted;

    constexpr big minValue = std::numeric_limits<Rep1>::min();
    constexpr big maxValue = std::numeric_limits<Rep1>::max();
    if(result < minValue || result > maxValue) {
      assert(false && "Result out of range for Rep type");
    }
  }
  return Quantity<UnitTag, Rep1, targetScale>{static_cast<Rep1>(result)};
}

template <typename UnitTag, typename Rep1, typename Scale1, typename Rep2, typename Scale2>
constexpr auto operator-(const Quantity<UnitTag, Rep1, Scale1> &lhs, const Quantity<UnitTag, Rep2, Scale2> &rhs) {
  static_assert(std::is_same<UnitTag, typename Quantity<UnitTag, Rep2, Scale2>::unit>::value,
                "UnitTags are not the same!");

  using big = std::intmax_t;
  using targetScale =
      typename std::conditional<(Scale1::num * Scale2::den < Scale2::num * Scale1::den), Scale1, Scale2>::type;
  big result;

  if constexpr(std::is_same<Scale1, Scale2>::value) {
    result = static_cast<big>(lhs.getRawValue()) + static_cast<big>(rhs.getRawValue());

    constexpr big minValue = std::numeric_limits<Rep1>::min();
    constexpr big maxValue = std::numeric_limits<Rep1>::max();
    if(result < minValue || result > maxValue) {
      assert(false && "Result out of range for Rep type");
    }
  } else {
    using lhs_ratio = std::ratio_divide<Scale1, targetScale>;
    using rhs_ratio = std::ratio_divide<Scale2, targetScale>;

    big lhs_adjusted = static_cast<big>(lhs.getRawValue()) * lhs_ratio::num / lhs_ratio::den;
    big rhs_adjusted = static_cast<big>(rhs.getRawValue()) * rhs_ratio::num / rhs_ratio::den;

    result = lhs_adjusted - rhs_adjusted;

    constexpr big minValue = std::numeric_limits<Rep1>::min();
    constexpr big maxValue = std::numeric_limits<Rep1>::max();
    if(result < minValue || result > maxValue) {
      assert(false && "Result out of range for Rep type");
    }
  }
  return Quantity<UnitTag, Rep1, targetScale>{static_cast<Rep1>(result)};
}