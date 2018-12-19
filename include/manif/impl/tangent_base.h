#ifndef _MANIF_MANIF_TANGENT_BASE_H_
#define _MANIF_MANIF_TANGENT_BASE_H_

#include "manif/impl/macro.h"
#include "manif/impl/traits.h"
#include "manif/impl/eigen.h"

#include "manif/constants.h"

#include "lt/optional.hpp"

namespace manif {

/**
 * @brief Base class for Lie groups' tangents.
 * @class Defines the minimum common API.
 * @see LieGroupBase.
 */
template <class _Derived>
struct TangentBase
{
  static constexpr int Dim     = internal::traits<_Derived>::Dim;
  static constexpr int RepSize = internal::traits<_Derived>::RepSize;
  static constexpr int DoF     = internal::traits<_Derived>::DoF;

  using Scalar   = typename internal::traits<_Derived>::Scalar;
  using LieGroup = typename internal::traits<_Derived>::LieGroup;
  using Tangent  = typename internal::traits<_Derived>::Tangent;
  using DataType = typename internal::traits<_Derived>::DataType;
  using Jacobian = typename internal::traits<_Derived>::Jacobian;
  using LieAlg   = typename internal::traits<_Derived>::LieAlg;

  using OptJacobianRef = tl::optional<Eigen::Ref<Jacobian>>;

  template <typename _Scalar>
  using TangentTemplate = typename internal::traitscast<Tangent, _Scalar>::cast;

public:

  TangentBase()  = default;
  ~TangentBase() = default;

  //! @brief Access the underlying data by reference
  DataType& coeffs();
  //! @brief Access the underlying data by const reference
  const DataType& coeffs() const;

  //! @brief Access the underlying data by pointer
  Scalar* data();
  //! @brief Access the underlying data by const pointer
  const Scalar* data() const;

  //! @brief Cast the Tangent object to a copy
  //! of a different scalar type
  template <class _NewScalar>
  TangentTemplate<_NewScalar> cast() const;

  // Common Tangent API

  /**
   * @brief Set the Tangent object this to Zero.
   * @return A reference to this.
   */
  _Derived& setZero();

  /**
   * @brief Set the LieGroup object this to a random value.
   * @return A reference to this.
   */
  _Derived& setRandom();

  // Minimum API
  // Those functions must be implemented in the Derived class !

  /**
   * @brief Hat operator of the Tangent element.
   * @return An element of the Lie algebra.
   * @note See Eq. (10).
   */
  LieAlg hat() const;

  /**
   * @brief Get the Lie group element
   * @param[out] -optional- J_m_t Jacobian of the Lie groupe element wrt this.
   * @return Associated Lie group element.
   * @note See Eq. (22).
   */
  LieGroup retract(OptJacobianRef J_m_t =
                    OptJacobianRef{}) const;

  /**
   * @brief Right oplus operation of the Lie group.
   * @param[in]  t An element of the tangent of the Lie group.
   * @param[out] -optional- J_mout_m Jacobian of the oplus operation wrt this.
   * @param[out] -optional- J_mout_t Jacobian of the oplus operation wrt the tangent element.
   * @return An element of the Lie group.
   * @note See Eq. (25).
   */
  LieGroup rplus(const LieGroup& m) const;

  /**
   * @brief Left oplus operation of the Lie group.
   * @param[in]  t An element of the tangent of the Lie group.
   * @param[out] -optional- J_mout_m Jacobian of the oplus operation wrt this.
   * @param[out] -optional- J_mout_t Jacobian of the oplus operation wrt the tangent element.
   * @return An element of the Lie group.
   * @note See Eq. (27).
   */
  LieGroup lplus(const LieGroup& m) const;

  /**
   * @brief An alias for the right oplus operation.
   * @see rplus
   */
  LieGroup plus(const LieGroup& m) const;

  template <typename _DerivedOther>
  Tangent plus(const TangentBase<_DerivedOther>& t) const;

  template <typename _DerivedOther>
  Tangent minus(const TangentBase<_DerivedOther>& t) const;

  /**
   * @brief Get the right Jacobian.
   * @note See Eq. (41).
   */
  Jacobian rjac() const;

  /**
   * @brief Get the left Jacobian.
   * @note See Eq. (44).
   */
  Jacobian ljac() const;

  /// @note Calls Derived's 'overload'
  template <typename U = _Derived>
  typename std::enable_if<
    internal::has_rjacinv<U>::value,
    typename TangentBase<U>::Jacobian>::type rjacinv() const;

  /// @note Calls Base default impl
  template <typename U = _Derived>
  typename std::enable_if<
    not internal::has_rjacinv<U>::value,
    typename TangentBase<U>::Jacobian>::type rjacinv() const;

  /// @note Calls Derived's 'overload'
  template <typename U = _Derived>
  typename std::enable_if<
    internal::has_ljacinv<U>::value,
    typename TangentBase<U>::Jacobian>::type ljacinv() const;

  /// @note Calls Base default impl
  template <typename U = _Derived>
  typename std::enable_if<
    not internal::has_ljacinv<U>::value,
    typename TangentBase<U>::Jacobian>::type ljacinv() const;

  /**
   * @brief
   * @return [description]
   */
  Jacobian smallAdj() const;

  /**
   * @brief Evaluate whether this and v are 'close'.
   * @details This evaluation is performed element-wise.
   * @param[in] v A vector.
   * @param[in] eps Threshold for equality copmarison.
   * @return true if the Tangent element t is 'close' to this,
   * false otherwise.
   */
  template <typename _EigenDerived>
  bool isApprox(const Eigen::MatrixBase<_EigenDerived>& v,
                const Scalar eps) const;

  /**
   * @brief Evaluate whether this and t are 'close'.
   * @details This evaluation is performed element-wise.
   * @param[in] t An element of the same Tangent group.
   * @param[in] eps Threshold for equality copmarison.
   * @return true if the Tangent element t is 'close' to this,
   * false otherwise.
   */
  template <typename _DerivedOther>
  bool isApprox(const TangentBase<_DerivedOther>& t,
                const Scalar eps) const;

  // Some operators

  // Copy assignment

  /**
   * @brief Assignment operator.
   * @param[in] t An element of the same Tangent group.
   * @return A reference to this.
   */
  _Derived& operator =(const TangentBase<_Derived>& t);

  /**
   * @brief Assignment operator.
   * @param[in] t An element of the same Tangent group.
   * @return A reference to this.
   */
  template <typename _DerivedOther>
  _Derived& operator =(const TangentBase<_DerivedOther>& t);

  /**
   * @brief Assignment operator.
   * @param[in] t A DataType object.
   * @return A reference to this.
   * @see DataType.
   */
  template <typename _EigenDerived>
  _Derived& operator =(const Eigen::MatrixBase<_EigenDerived>& v);

  // Math

  //! @brief Equivalent to -1 * v.
  Tangent operator -() const;

  /**
   * @brief Left oplus operator.
   * @see lplus.
   */
  LieGroup operator +(const LieGroup& m) const;

  //! @brief In-place plus operator, simple vector in-place plus operation.
  template <typename _DerivedOther>
  _Derived& operator +=(const TangentBase<_DerivedOther>& t);

  //! @brief In-place minus operator, simple vector in-place minus operation.
  template <typename _DerivedOther>
  _Derived& operator -=(const TangentBase<_DerivedOther>& t);

  //! @brief In-place plus operator, simple vector in-place plus operation.
  template <typename _EigenDerived>
  _Derived& operator +=(const Eigen::MatrixBase<_EigenDerived>& v);

  //! @brief In-place minus operator, simple vector in-place minus operation.
  template <typename _EigenDerived>
  _Derived& operator -=(const Eigen::MatrixBase<_EigenDerived>& v);

  //! @brief Multiply the underlying vector with a scalar.
  template <typename T>
  Tangent operator *=(const T scalar);

  //! @brief Divide the underlying vector with a scalar.
  template <typename T>
  Tangent operator /=(const T scalar);

  // static helpers

  //! Static helper the create a Tangent object set to Zero.
  static Tangent Zero();
  //! Static helper the create a random Tangent object.
  static Tangent Random();

private:

  _Derived& derived() { return *static_cast< _Derived* >(this); }
  const _Derived& derived() const { return *static_cast< const _Derived* >(this); }
};

template <typename _Derived>
typename TangentBase<_Derived>::DataType&
TangentBase<_Derived>::coeffs()
{
  return derived().coeffs();
}

template <typename _Derived>
const typename TangentBase<_Derived>::DataType&
TangentBase<_Derived>::coeffs() const
{
  return derived().coeffs();
}

template <class _Derived>
typename TangentBase<_Derived>::Scalar*
TangentBase<_Derived>::data()
{
  return derived().coeffs().data();
}

template <class _Derived>
const typename TangentBase<_Derived>::Scalar*
TangentBase<_Derived>::data() const
{
  return derived().coeffs().data();
}

template <typename _Derived>
template <class _NewScalar>
typename TangentBase<_Derived>::template TangentTemplate<_NewScalar>
TangentBase<_Derived>::cast() const
{
  return TangentTemplate<_NewScalar>(coeffs().template cast<_NewScalar>());
}

template <class _Derived>
_Derived& TangentBase<_Derived>::setZero()
{
  coeffs().setZero();
  return derived();
}

template <class _Derived>
_Derived& TangentBase<_Derived>::setRandom()
{
  coeffs().setRandom();
  return derived();
}

template <class _Derived>
typename TangentBase<_Derived>::LieGroup
TangentBase<_Derived>::retract(OptJacobianRef J_m_t) const
{
  return derived().retract(J_m_t);
}

template <class _Derived>
typename TangentBase<_Derived>::LieAlg
TangentBase<_Derived>::hat() const
{
  return derived().hat();
}

template <class _Derived>
typename TangentBase<_Derived>::LieGroup
TangentBase<_Derived>::rplus(const LieGroup& m) const
{
  return m.rplus(derived());
}

template <class _Derived>
typename TangentBase<_Derived>::LieGroup
TangentBase<_Derived>::lplus(const LieGroup& m) const
{
  return m.lplus(derived());
}

template <class _Derived>
typename TangentBase<_Derived>::LieGroup
TangentBase<_Derived>::plus(const LieGroup& m) const
{
  return m.lplus(derived());
}

template <class _Derived>
template <typename _DerivedOther>
typename TangentBase<_Derived>::Tangent
TangentBase<_Derived>::plus(const TangentBase<_DerivedOther>& t) const
{
  return *this + t;
}

template <class _Derived>
template <typename _DerivedOther>
typename TangentBase<_Derived>::Tangent
TangentBase<_Derived>::minus(const TangentBase<_DerivedOther>& t) const
{
  return *this - t;
}

template <class _Derived>
typename TangentBase<_Derived>::Jacobian
TangentBase<_Derived>::rjac() const
{
  return derived().rjac();
}

template <class _Derived>
typename TangentBase<_Derived>::Jacobian
TangentBase<_Derived>::ljac() const
{
  return derived().ljac();
}


template <class _Derived>
template <typename U>
typename std::enable_if<
  internal::has_rjacinv<U>::value,
  typename TangentBase<U>::Jacobian>::type
TangentBase<_Derived>::rjacinv() const
{
  return derived().rjacinv();
}

template <class _Derived>
template <typename U>
typename std::enable_if<
  not internal::has_rjacinv<U>::value,
  typename TangentBase<U>::Jacobian>::type
TangentBase<_Derived>::rjacinv() const
{
  return derived().rjac().inverse();
}

template <class _Derived>
template <typename U>
typename std::enable_if<
  internal::has_ljacinv<U>::value,
  typename TangentBase<U>::Jacobian>::type
TangentBase<_Derived>::ljacinv() const
{
  return derived().ljacinv();
}

template <class _Derived>
template <typename U>
typename std::enable_if<
  not internal::has_ljacinv<U>::value,
  typename TangentBase<U>::Jacobian>::type
TangentBase<_Derived>::ljacinv() const
{
  return derived().ljac().inverse();
}

template <class _Derived>
typename TangentBase<_Derived>::Jacobian
TangentBase<_Derived>::smallAdj() const
{
  return derived().smallAdj();
}

template <typename _Derived>
template <typename _EigenDerived>
bool TangentBase<_Derived>::isApprox(
    const Eigen::MatrixBase<_EigenDerived>& t,
    const Scalar eps) const
{
  using std::min;
  bool result = false;

  if (min(coeffs().norm(), t.norm()) < eps)
  {
    result = ((coeffs() - t).isZero(eps));
  }
  else
  {
    result = (coeffs().isApprox(t, eps));
  }

  return result;
}

template <typename _Derived>
template <typename _DerivedOther>
bool TangentBase<_Derived>::isApprox(
    const TangentBase<_DerivedOther>& t,
    const Scalar eps) const
{
  return isApprox(t.coeffs(), eps);
}

// Operators

// Copy assignment

template <typename _Derived>
_Derived&
TangentBase<_Derived>::operator =(
    const TangentBase<_Derived>& t)
{
  coeffs() = t.coeffs();
  return derived();
}

template <typename _Derived>
template <typename _DerivedOther>
_Derived&
TangentBase<_Derived>::operator =(
    const TangentBase<_DerivedOther>& t)
{
  coeffs() = t.coeffs();
  return derived();
}

template <typename _Derived>
template <typename _EigenDerived>
_Derived& TangentBase<_Derived>::operator =(
    const Eigen::MatrixBase<_EigenDerived>& v)
{
  coeffs() = v;
  return derived();
}

// Static helper

template <class _Derived>
typename TangentBase<_Derived>::Tangent
TangentBase<_Derived>::Zero()
{
  static const Tangent t(DataType::Zero());
  return t;
}

template <class _Derived>
typename TangentBase<_Derived>::Tangent
TangentBase<_Derived>::Random()
{
  return Tangent().setRandom();
}

// Math

template <typename _Derived>
typename TangentBase<_Derived>::Tangent
TangentBase<_Derived>::operator -() const
{
  return Tangent(-coeffs());
}

template <typename _Derived>
typename TangentBase<_Derived>::LieGroup
TangentBase<_Derived>::operator +(const LieGroup& m) const
{
  return m.lplus(derived());
}

template <typename _Derived>
template <typename _DerivedOther>
_Derived& TangentBase<_Derived>::operator +=(
    const TangentBase<_DerivedOther>& t)
{
  coeffs() += t.coeffs();
  return derived();
}

template <typename _Derived>
template <typename _DerivedOther>
_Derived& TangentBase<_Derived>::operator -=(
    const TangentBase<_DerivedOther>& t)
{
  coeffs() -= t.coeffs();
  return derived();
}

template <typename _Derived, typename _DerivedOther>
typename TangentBase<_Derived>::Tangent
operator +(const TangentBase<_Derived>& ta,
           const TangentBase<_DerivedOther>& tb)
{
  typename TangentBase<_Derived>::Tangent tc(ta);
  return tc += tb;
}

template <typename _Derived, typename _DerivedOther>
typename TangentBase<_Derived>::Tangent
operator -(const TangentBase<_Derived>& ta,
           const TangentBase<_DerivedOther>& tb)
{
  typename TangentBase<_Derived>::Tangent tc(ta);
  return tc -= tb;
}

template <typename _Derived>
template <typename _EigenDerived>
_Derived& TangentBase<_Derived>::operator +=(
    const Eigen::MatrixBase<_EigenDerived>& v)
{
  coeffs() += v;
  return derived();
}

template <typename _Derived>
template <typename _EigenDerived>
_Derived& TangentBase<_Derived>::operator -=(
    const Eigen::MatrixBase<_EigenDerived>& v)
{
  coeffs() -= v;
  return derived();
}

template <typename _Derived, typename _EigenDerived>
typename TangentBase<_Derived>::Tangent
operator +(const TangentBase<_Derived>& t,
           const Eigen::MatrixBase<_EigenDerived>& v)
{
  typename TangentBase<_Derived>::Tangent ret(t);
  return ret += v;
}

template <typename _Derived, typename _EigenDerived>
typename TangentBase<_Derived>::Tangent
operator -(const TangentBase<_Derived>& t,
           const Eigen::MatrixBase<_EigenDerived>& v)
{
  typename TangentBase<_Derived>::Tangent ret(t);
  return ret -= v;
}

template <typename _EigenDerived, typename _Derived>
_EigenDerived
operator +(const Eigen::MatrixBase<_EigenDerived>& v,
           const TangentBase<_Derived>& t)
{
  _EigenDerived ret(v);
  return ret += t.coeffs();
}

template <typename _EigenDerived, typename _Derived>
_EigenDerived
operator -(const TangentBase<_Derived>& t,
           const Eigen::MatrixBase<_EigenDerived>& v)
{
  _EigenDerived ret(v);
  return ret -= t.coeffs();
}

template <typename _Derived>
template <typename T>
typename TangentBase<_Derived>::Tangent
TangentBase<_Derived>::operator *=(const T scalar)
{
  coeffs() *= scalar;
  return derived();
}

template <typename _Derived>
template <typename T>
typename TangentBase<_Derived>::Tangent
TangentBase<_Derived>::operator /=(const T scalar)
{
  coeffs() /= scalar;
  return derived();
}

template <typename _Derived, typename T>
typename TangentBase<_Derived>::Tangent
operator *(const TangentBase<_Derived>& t,
           const T scalar)
{
  typename TangentBase<_Derived>::Tangent ret(t);
  return ret *= scalar;
}

template <typename _Derived, typename T>
typename TangentBase<_Derived>::Tangent
operator /(const TangentBase<_Derived>& t,
           const T scalar)
{
  typename TangentBase<_Derived>::Tangent ret(t);
  return ret /= scalar;
}

template <class _DerivedOther>
typename TangentBase<_DerivedOther>::Tangent
operator *(const typename TangentBase<_DerivedOther>::Jacobian& J,
           const TangentBase<_DerivedOther>& t)
{
  return typename TangentBase<_DerivedOther>::Tangent(
        typename TangentBase<_DerivedOther>::DataType(J*t.coeffs()));
}

template <typename _Derived, typename _DerivedOther>
bool operator ==(
    const TangentBase<_Derived>& ta,
    const TangentBase<_DerivedOther>& tb)
{
  return ta.isApprox(tb, Constants<typename TangentBase<_Derived>::Scalar>::eps);
}

template <typename _Derived, typename _EigenDerived>
bool operator ==(
    const TangentBase<_Derived>& t,
    const Eigen::MatrixBase<_EigenDerived>& v)
{
  return t.isApprox(v, Constants<typename TangentBase<_Derived>::Scalar>::eps);
}

/// Utils

template <typename _Stream, typename _Derived>
_Stream& operator << (
    _Stream& s,
    const manif::TangentBase<_Derived>& m)
{
  s << m.coeffs().transpose();
  return s;
}

} /* namespace manif */

#endif /* _MANIF_MANIF_TANGENT_BASE_H_ */
