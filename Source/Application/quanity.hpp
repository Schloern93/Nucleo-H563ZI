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

  template <typename UnitTagFriend, typename RepFriend, typename Scale1, typename Scale2>
  friend constexpr auto operator+(const Quantity<UnitTagFriend, RepFriend, Scale1> &lhs,
                                  const Quantity<UnitTagFriend, RepFriend, Scale2> &rhs);

  template <typename UnitTagFriend, typename RepFriend, typename Scale1, typename Scale2>
  friend constexpr auto operator-(const Quantity<UnitTagFriend, RepFriend, Scale1> &lhs,
                                  const Quantity<UnitTagFriend, RepFriend, Scale2> &rhs);

  template <typename UnitTag1, typename Rep1, typename Scale1, typename UnitTag2, typename Rep2, typename Scale2>
  friend constexpr auto operator==(const Quantity<UnitTag1, Rep1, Scale1> &lhs,
                                   const Quantity<UnitTag2, Rep2, Scale2> &rhs);

private:
  rep raw;
};

template <typename UnitTag1, typename Rep1, typename Scale1, typename UnitTag2, typename Rep2, typename Scale2>
constexpr bool operator==(const Quantity<UnitTag1, Rep1, Scale1> &lhs, const Quantity<UnitTag2, Rep2, Scale2> &rhs) {
  return std::is_same<UnitTag1, UnitTag2>::value && std::is_same<Rep1, Rep2>::value &&
         std::is_same<Scale1, Scale2>::value && lhs.raw == rhs.raw;
}

template <typename UnitTagFriend, typename RepFriend, typename Scale1, typename Scale2>
constexpr auto operator+(const Quantity<UnitTagFriend, RepFriend, Scale1> &lhs,
                         const Quantity<UnitTagFriend, RepFriend, Scale2> &rhs) {
  static_assert(std::is_same<UnitTagFriend, typename Quantity<UnitTagFriend, RepFriend, Scale2>::unit>::value,
                "UnitTags are not the same!");

  using big = std::intmax_t;
  using targetScale =
      typename std::conditional<(Scale1::num * Scale2::den < Scale2::num * Scale1::den), Scale1, Scale2>::type;
  big result;

  constexpr big minValue = std::numeric_limits<RepFriend>::min();
  constexpr big maxValue = std::numeric_limits<RepFriend>::max();

  if constexpr(std::is_same<Scale1, Scale2>::value) {
    result = static_cast<big>(lhs.raw) + static_cast<big>(rhs.raw);

    if(result < minValue || result > maxValue) {
      assert(false && "Result out of range for Rep type");
    }
  } else {
    using lhs_ratio = std::ratio_divide<Scale1, targetScale>;
    using rhs_ratio = std::ratio_divide<Scale2, targetScale>;

    big lhs_adjusted = static_cast<big>(lhs.raw) * lhs_ratio::num / lhs_ratio::den;
    big rhs_adjusted = static_cast<big>(rhs.raw) * rhs_ratio::num / rhs_ratio::den;

    result = lhs_adjusted + rhs_adjusted;

    if(result < minValue || result > maxValue) {
      assert(false && "Result out of range for Rep type");
    }
  }
  return Quantity<UnitTagFriend, RepFriend, targetScale>{static_cast<RepFriend>(result)};
}

template <typename UnitTagFriend, typename RepFriend, typename Scale1, typename Scale2>
constexpr auto operator-(const Quantity<UnitTagFriend, RepFriend, Scale1> &lhs,
                         const Quantity<UnitTagFriend, RepFriend, Scale2> &rhs) {
  static_assert(std::is_same<UnitTagFriend, typename Quantity<UnitTagFriend, RepFriend, Scale2>::unit>::value,
                "UnitTags are not the same!");

  using big = std::intmax_t;
  using targetScale =
      typename std::conditional<(Scale1::num * Scale2::den < Scale2::num * Scale1::den), Scale1, Scale2>::type;
  big result;

  constexpr big minValue = std::numeric_limits<RepFriend>::min();
  constexpr big maxValue = std::numeric_limits<RepFriend>::max();

  if constexpr(std::is_same<Scale1, Scale2>::value) {
    result = static_cast<big>(lhs.raw) + static_cast<big>(rhs.raw);

    if(result < minValue || result > maxValue) {
      assert(false && "Result out of range for Rep type");
    }
  } else {
    using lhs_ratio = std::ratio_divide<Scale1, targetScale>;
    using rhs_ratio = std::ratio_divide<Scale2, targetScale>;

    big lhs_adjusted = static_cast<big>(lhs.raw) * lhs_ratio::num / lhs_ratio::den;
    big rhs_adjusted = static_cast<big>(rhs.raw) * rhs_ratio::num / rhs_ratio::den;

    result = lhs_adjusted - rhs_adjusted;

    if(result < minValue || result > maxValue) {
      assert(false && "Result out of range for Rep type");
    }
  }
  return Quantity<UnitTagFriend, RepFriend, targetScale>{static_cast<RepFriend>(result)};
}