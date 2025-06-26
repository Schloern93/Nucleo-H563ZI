#pragma once
#include <cstdint>
#include <ratio>
#include <type_traits>

/* -----------  generischer Wert mit Einheit und Skalierungsfaktor  ----------- */
template <typename UnitTag,                 // z. B. Celsius
          typename Rep = int32_t,           // zugrundeliegender Integer
          typename Scale = std::ratio<1, 1> // phys. Einheit : 1 Digit
          >
class Quantity {
public:
  using rep = Rep;
  using unit = UnitTag;
  using scale = Scale;

  /* Rohwert-Konstruktor (z. B. 253  ⇒  25,3 °C bei Scale 1/10) */
  constexpr explicit Quantity(rep raw = 0)
      : raw_{raw} {
  }

  /* Zugriff auf den unveränderten Integerwert */
  constexpr rep raw() const {
    return raw_;
  }

  /* Rechnen ist nur mit identischem UnitTag & Scale erlaubt */
  constexpr Quantity operator+(Quantity rhs) const {
    return Quantity{raw_ + rhs.raw_};
  }
  constexpr Quantity operator-(Quantity rhs) const {
    return Quantity{raw_ - rhs.raw_};
  }

private:
  rep raw_;
};

/* -----------  Skalen­umrechnung innerhalb der **gleichen** Einheit  ----------- */
template <typename OutQ, typename InQ> constexpr OutQ quantity_cast(InQ in) {
  /* dürfen nur dieselbe Einheit haben – sonst compile-error */
  static_assert(std::is_same<typename OutQ::unit, typename InQ::unit>::value,
                "quantity_cast: UnitTags unterschiedlich!");

  /* Verhältnis der Skalen berechnen:   Scale_in / Scale_out   */
  using ratio = std::ratio_divide<typename InQ::scale, typename OutQ::scale>;
  using big = std::intmax_t;

  big tmp = static_cast<big>(in.raw()) * ratio::num;
  tmp /= ratio::den;
  return OutQ{static_cast<typename OutQ::rep>(tmp)};
}
