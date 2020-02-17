#ifndef _MANIF_MANIF_SO3_H_
#define _MANIF_MANIF_SO3_H_

namespace manif {

// Forward declare for type traits specialization

template <typename _Scalar> struct SO3;
template <typename _Scalar> struct SO3Tangent;

namespace internal {

//! Traits specialization
template <typename _Scalar>
struct traits<SO3<_Scalar>>
{
  using Scalar = _Scalar;

  using LieGroup = SO3<_Scalar>;
  using Tangent  = SO3Tangent<_Scalar>;

  using Base = SO3Base<SO3<_Scalar>>;

  static constexpr int Dim     = LieGroupProperties<Base>::Dim;
  static constexpr int DoF     = LieGroupProperties<Base>::DoF;
  static constexpr int RepSize = 4;

  using DataType       = Eigen::Matrix<Scalar, RepSize, 1>;

  using Jacobian       = Eigen::Matrix<Scalar, DoF, DoF>;
  using Transformation = Eigen::Matrix<Scalar, 4, 4>;
  using Rotation       = Eigen::Matrix<Scalar, Dim, Dim>;
  using Vector         = Eigen::Matrix<Scalar, Dim, 1>;
};

} /* namespace internal */
} /* namespace manif */

namespace manif {

//
// LieGroup
//

/**
 * @brief Represents an element of SO3.
 */
template <typename _Scalar>
struct SO3 : SO3Base<SO3<_Scalar>>
{
private:

  using Base = SO3Base<SO3<_Scalar>>;
  using Type = SO3<_Scalar>;

protected:

  using Base::derived;

public:

  MANIF_MAKE_ALIGNED_OPERATOR_NEW_COND

  MANIF_GROUP_TYPEDEF
  using Base::Rotation;
  using Base::Transformation;
  // using Base::QuaternionDataType;
  using QuaternionDataType = Eigen::Quaternion<Scalar>;

  SO3()  = default;
  ~SO3() = default;

  // Copy constructor
  MANIF_COPY_CONSTRUCTOR(SO3)
  template <typename _DerivedOther>
  SO3(const LieGroupBase<_DerivedOther>& o);

  /**
   * @brief Constructor given a unit quaternion.
   * @param[in] q A unit quaternion.
   * @throws manif::invalid_argument on un-normalized quaternion.
   */
  SO3(const QuaternionDataType& q);

  /**
   * @brief Constructor given the quaternion's coefficients.
   * @param[in] x The x-components of a unit quaternion.
   * @param[in] y The x-components of a unit quaternion.
   * @param[in] z The x-components of a unit quaternion.
   * @param[in] w The x-components of a unit quaternion.
   * @throws manif::invalid_argument on un-normalized quaternion.
   */
  SO3(const Scalar x, const Scalar y,
      const Scalar z, const Scalar w);

  //! @brief Constructor given an angle axis.
  SO3(const Eigen::AngleAxis<Scalar>& angle_axis);

  //! @brief Constructor given Euler angles.
  SO3(const Scalar roll, const Scalar pitch, const Scalar yaw);

  MANIF_GROUP_API
  using Base::data;

  MANIF_COEFFS_FUNCTIONS

  MANIF_GROUP_ASSIGN_OP(SO3)
  MANIF_GROUP_OPERATOR

  using Base::transform;
  using Base::rotation;
  using Base::quat;
  using Base::normalize;
  using Base::x;
  using Base::y;
  using Base::z;
  using Base::w;

protected:

  DataType data_;
};

MANIF_EXTRA_GROUP_TYPEDEF(SO3)

template <typename _Scalar>
template <typename _DerivedOther>
SO3<_Scalar>::SO3(const LieGroupBase<_DerivedOther>& o)
  : SO3(o.coeffs())
{
  //
}

template <typename _Scalar>
SO3<_Scalar>::SO3(const QuaternionDataType& q)
  : SO3(q.coeffs())
{
  //
}

template <typename _Scalar>
SO3<_Scalar>::SO3(const Scalar x, const Scalar y,
                  const Scalar z, const Scalar w)
  : SO3((DataType() << x, y, z, w).finished())
{
  //
}

template <typename _Scalar>
SO3<_Scalar>::SO3(const Eigen::AngleAxis<Scalar>& angle_axis)
  : SO3(QuaternionDataType(angle_axis).coeffs())
{

}

template <typename _Scalar>
SO3<_Scalar>::SO3(const Scalar roll,
                  const Scalar pitch,
                  const Scalar yaw)
  : SO3(Eigen::AngleAxis<Scalar>(yaw,   Eigen::Matrix<Scalar, 3, 1>::UnitZ()) *
        Eigen::AngleAxis<Scalar>(pitch, Eigen::Matrix<Scalar, 3, 1>::UnitY()) *
        Eigen::AngleAxis<Scalar>(roll,  Eigen::Matrix<Scalar, 3, 1>::UnitX())  )
{
  //
}

} /* namespace manif */

#endif /* _MANIF_MANIF_SO3_H_ */
