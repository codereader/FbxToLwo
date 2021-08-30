#include "Matrix4.h"

#include "Quaternion.h"

#include <sstream>

namespace
{
	/// \brief Returns \p euler angles converted from degrees to radians.
	inline Vector3 euler_degrees_to_radians(const Vector3& euler)
	{
		return Vector3(
			degrees_to_radians(euler.x()),
			degrees_to_radians(euler.y()),
			degrees_to_radians(euler.z())
		);
	}

	inline bool quaternion_component_is_90(double component)
	{
		return (fabs(component) - c_half_sqrt2f) < 0.001f;
	}
}

// Main explicit constructor (private)
Matrix4::Matrix4(double xx_, double xy_, double xz_, double xw_,
						double yx_, double yy_, double yz_, double yw_,
						double zx_, double zy_, double zz_, double zw_,
						double tx_, double ty_, double tz_, double tw_)
{
    xx() = xx_;
    xy() = xy_;
    xz() = xz_;
    xw() = xw_;
    yx() = yx_;
    yy() = yy_;
    yz() = yz_;
    yw() = yw_;
    zx() = zx_;
    zy() = zy_;
    zz() = zz_;
    zw() = zw_;
    tx() = tx_;
    ty() = ty_;
    tz() = tz_;
    tw() = tw_;
}

// Named constructors

// Identity matrix
const Matrix4& Matrix4::getIdentity()
{
    static const Matrix4 _identity(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    );
    return _identity;
}

// Get a translation matrix for the given vector
Matrix4 Matrix4::getTranslation(const Vector3& translation)
{
    return Matrix4::byColumns(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        translation.x(), translation.y(), translation.z(), 1
    );
}

// Get a rotation from 2 vectors (named constructor)
Matrix4 Matrix4::getRotation(const Vector3& a, const Vector3& b)
{
	double angle = a.angle(b);
	Vector3 axis = b.crossProduct(a).getNormalised();

	return getRotation(axis, angle);
}

Matrix4 Matrix4::getRotation(const Vector3& axis, const double angle)
{
	// Pre-calculate the terms
	double cosPhi = cos(angle);
	double sinPhi = sin(angle);
	double oneMinusCosPhi = static_cast<double>(1) - cos(angle);
	double x = axis.x();
	double y = axis.y();
	double z = axis.z();
	return Matrix4::byColumns(
		cosPhi + oneMinusCosPhi*x*x, oneMinusCosPhi*x*y - sinPhi*z, oneMinusCosPhi*x*z + sinPhi*y, 0,
		oneMinusCosPhi*y*x + sinPhi*z, cosPhi + oneMinusCosPhi*y*y, oneMinusCosPhi*y*z - sinPhi*x, 0,
		oneMinusCosPhi*z*x - sinPhi*y, oneMinusCosPhi*z*y + sinPhi*x, cosPhi + oneMinusCosPhi*z*z, 0,
		0, 0, 0, 1
	);
}

Matrix4 Matrix4::getRotation(const Quaternion& quaternion)
{
	const double x2 = quaternion[0] + quaternion[0];
	const double y2 = quaternion[1] + quaternion[1];
	const double z2 = quaternion[2] + quaternion[2];
	const double xx = quaternion[0] * x2;
	const double xy = quaternion[0] * y2;
	const double xz = quaternion[0] * z2;
	const double yy = quaternion[1] * y2;
	const double yz = quaternion[1] * z2;
	const double zz = quaternion[2] * z2;
	const double wx = quaternion[3] * x2;
	const double wy = quaternion[3] * y2;
	const double wz = quaternion[3] * z2;

	return Matrix4::byColumns(
		1.0f - (yy + zz),
		xy + wz,
		xz - wy,
		0,
		xy - wz,
		1.0f - (xx + zz),
		yz + wx,
		0,
		xz + wy,
		yz - wx,
		1.0f - (xx + yy),
		0,
		0,
		0,
		0,
		1
	);
}

Matrix4 Matrix4::getRotationQuantised(const Quaternion& quaternion)
{
	if (quaternion.y() == 0 && quaternion.z() == 0 && quaternion_component_is_90(quaternion.x()) && quaternion_component_is_90(quaternion.w()))
	{
		return Matrix4::getRotationAboutXForSinCos((quaternion.x() > 0) ? 1.0f : -1.0f, 0);
	}

	if (quaternion.x() == 0 && quaternion.z() == 0 && quaternion_component_is_90(quaternion.y()) && quaternion_component_is_90(quaternion.w()))
	{
		return Matrix4::getRotationAboutYForSinCos((quaternion.y() > 0) ? 1.0f : -1.0f, 0);
	}

	if (quaternion.x() == 0	&& quaternion.y() == 0 && quaternion_component_is_90(quaternion.z()) && quaternion_component_is_90(quaternion.w()))
	{
		return Matrix4::getRotationAboutZForSinCos((quaternion.z() > 0) ? 1.0f : -1.0f, 0);
	}

	return getRotation(quaternion);
}

Matrix4 Matrix4::getRotationAboutXForSinCos(double s, double c)
{
	return Matrix4::byColumns(
		1, 0, 0, 0,
		0, c, s, 0,
		0,-s, c, 0,
		0, 0, 0, 1
	);
}

Matrix4 Matrix4::getRotationAboutYForSinCos(double s, double c)
{
	return Matrix4::byColumns(
		c, 0,-s, 0,
		0, 1, 0, 0,
		s, 0, c, 0,
		0, 0, 0, 1
	);
}

Matrix4 Matrix4::getRotationAboutZForSinCos(double s, double c)
{
	return Matrix4::byColumns(
		c, s, 0, 0,
		-s, c, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
}

/*! \verbatim
clockwise rotation around X, Y, Z, facing along axis
 1  0   0    cy 0 -sy   cz  sz 0
 0  cx  sx   0  1  0   -sz  cz 0
 0 -sx  cx   sy 0  cy   0   0  1

rows of Z by cols of Y
 cy*cz -sy*cz+sz -sy*sz+cz
-sz*cy -sz*sy+cz

  .. or something like that..

final rotation is Z * Y * X
 cy*cz -sx*-sy*cz+cx*sz  cx*-sy*sz+sx*cz
-cy*sz  sx*sy*sz+cx*cz  -cx*-sy*sz+sx*cz
 sy    -sx*cy            cx*cy

transposed
cy.cz + 0.sz + sy.0            cy.-sz + 0 .cz +  sy.0          cy.0  + 0 .0  +   sy.1       |
sx.sy.cz + cx.sz + -sx.cy.0    sx.sy.-sz + cx.cz + -sx.cy.0    sx.sy.0  + cx.0  + -sx.cy.1  |
-cx.sy.cz + sx.sz +  cx.cy.0   -cx.sy.-sz + sx.cz +  cx.cy.0   -cx.sy.0  + 0 .0  +  cx.cy.1  |
\endverbatim */
Matrix4 Matrix4::getRotationForEulerXYZ(const Vector3& euler)
{
	double cx = cos(euler[0]);
	double sx = sin(euler[0]);
	double cy = cos(euler[1]);
	double sy = sin(euler[1]);
	double cz = cos(euler[2]);
	double sz = sin(euler[2]);

	return Matrix4::byColumns(
		cy*cz,
		cy*sz,
		-sy,
		0,
		sx*sy*cz + cx*-sz,
		sx*sy*sz + cx*cz,
		sx*cy,
		0,
		cx*sy*cz + sx*sz,
		cx*sy*sz + -sx*cz,
		cx*cy,
		0,
		0,
		0,
		0,
		1
	);
}

Matrix4 Matrix4::getRotationForEulerXYZDegrees(const Vector3& euler)
{
	return getRotationForEulerXYZ(euler_degrees_to_radians(euler));
}

// Get a scale matrix
Matrix4 Matrix4::getScale(const Vector3& scale)
{
    return Matrix4::byColumns(
        scale[0], 0, 0, 0,
        0, scale[1], 0, 0,
        0, 0, scale[2], 0,
        0, 0, 0,        1
    );
}

// Transpose the matrix in-place
void Matrix4::transpose()
{
    std::swap(_m[1], _m[4]); // xy <=> yx
    std::swap(_m[2], _m[8]); // xz <=> zx
    std::swap(_m[3], _m[12]); // xw <=> tx
    std::swap(_m[6], _m[9]); // yz <=> zy
    std::swap(_m[7], _m[13]); // yw <=> ty
    std::swap(_m[11], _m[14]); // zw <=> tz
}

// Return transposed copy
Matrix4 Matrix4::getTransposed() const
{
    return Matrix4(
        xx(), yx(), zx(), tx(),
        xy(), yy(), zy(), ty(),
        xz(), yz(), zz(), tz(),
        xw(), yw(), zw(), tw()
    );
}

// Return affine inverse
Matrix4 Matrix4::getInverse() const
{
  Matrix4 result;

  // determinant of rotation submatrix
  double det
    = _m[0] * ( _m[5]*_m[10] - _m[9]*_m[6] )
    - _m[1] * ( _m[4]*_m[10] - _m[8]*_m[6] )
    + _m[2] * ( _m[4]*_m[9] - _m[8]*_m[5] );

  // throw exception here if (det*det < 1e-25)

  // invert rotation submatrix
  det = 1.0f / det;

  result[0] = (  (_m[5]*_m[10]- _m[6]*_m[9] )*det);
  result[1] = (- (_m[1]*_m[10]- _m[2]*_m[9] )*det);
  result[2] = (  (_m[1]*_m[6] - _m[2]*_m[5] )*det);
  result[3] = 0;
  result[4] = (- (_m[4]*_m[10]- _m[6]*_m[8] )*det);
  result[5] = (  (_m[0]*_m[10]- _m[2]*_m[8] )*det);
  result[6] = (- (_m[0]*_m[6] - _m[2]*_m[4] )*det);
  result[7] = 0;
  result[8] = (  (_m[4]*_m[9] - _m[5]*_m[8] )*det);
  result[9] = (- (_m[0]*_m[9] - _m[1]*_m[8] )*det);
  result[10]= (  (_m[0]*_m[5] - _m[1]*_m[4] )*det);
  result[11] = 0;

  // multiply translation part by rotation
  result[12] = - (_m[12] * result[0] +
    _m[13] * result[4] +
    _m[14] * result[8]);
  result[13] = - (_m[12] * result[1] +
    _m[13] * result[5] +
    _m[14] * result[9]);
  result[14] = - (_m[12] * result[2] +
    _m[13] * result[6] +
    _m[14] * result[10]);
  result[15] = 1;

  return result;
}

Matrix4 Matrix4::getFullInverse() const
{
	// The inverse is generated through the adjugate matrix

	// 2x2 minors (re-usable for the determinant)
	double minor01 = zz() * tw() - zw() * tz();
	double minor02 = zy() * tw() - zw() * ty();
	double minor03 = zx() * tw() - zw() * tx();
	double minor04 = zy() * tz() - zz() * ty();
	double minor05 = zx() * tz() - zz() * tx();
	double minor06 = zx() * ty() - zy() * tx();

	// 2x2 minors (not usable for the determinant)
	double minor07 = yz() * tw() - yw() * tz();
	double minor08 = yy() * tw() - yw() * ty();
	double minor09 = yy() * tz() - yz() * ty();
	double minor10 = yx() * tw() - yw() * tx();
	double minor11 = yx() * tz() - yz() * tx();
	double minor12 = yx() * ty() - yy() * tx();
	double minor13 = yz() * zw() - yw() * zz();
	double minor14 = yy() * zw() - yw() * zy();
	double minor15 = yy() * zz() - yz() * zy();
	double minor16 = yx() * zw() - yw() * zx();
	double minor17 = yx() * zz() - yz() * zx();
	double minor18 = yx() * zy() - yy() * zx();

	// 3x3 minors (re-usable for the determinant)
	double minor3x3_11 = yy() * minor01 - yz() * minor02 + yw() * minor04;
	double minor3x3_21 = yx() * minor01 - yz() * minor03 + yw() * minor05;
	double minor3x3_31 = yx() * minor02 - yy() * minor03 + yw() * minor06;
	double minor3x3_41 = yx() * minor04 - yy() * minor05 + yz() * minor06;

	// 3x3 minors (not usable for the determinant)
	double minor3x3_12 = xy() * minor01 - xz() * minor02 + xw() * minor04;
	double minor3x3_22 = xx() * minor01 - xz() * minor03 + xw() * minor05;
	double minor3x3_32 = xx() * minor02 - xy() * minor03 + xw() * minor06;
	double minor3x3_42 = xx() * minor04 - xy() * minor05 + xz() * minor06;

	double minor3x3_13 = xy() * minor07 - xz() * minor08 + xw() * minor09;
	double minor3x3_23 = xx() * minor07 - xz() * minor10 + xw() * minor11;
	double minor3x3_33 = xx() * minor08 - xy() * minor10 + xw() * minor12;
	double minor3x3_43 = xx() * minor09 - xy() * minor11 + xz() * minor12;

	double minor3x3_14 = xy() * minor13 - xz() * minor14 + xw() * minor15;
	double minor3x3_24 = xx() * minor13 - xz() * minor16 + xw() * minor17;
	double minor3x3_34 = xx() * minor14 - xy() * minor16 + xw() * minor18;
	double minor3x3_44 = xx() * minor15 - xy() * minor17 + xz() * minor18;

	double determinant = xx() * minor3x3_11 - xy() * minor3x3_21 + xz() * minor3x3_31 - xw() * minor3x3_41;
	double invDet = 1.0f / determinant;

	return Matrix4::byColumns(
		+minor3x3_11 * invDet, -minor3x3_12 * invDet, +minor3x3_13 * invDet, -minor3x3_14 * invDet,
		-minor3x3_21 * invDet, +minor3x3_22 * invDet, -minor3x3_23 * invDet, +minor3x3_24 * invDet,
		+minor3x3_31 * invDet, -minor3x3_32 * invDet, +minor3x3_33 * invDet, -minor3x3_34 * invDet,
		-minor3x3_41 * invDet, +minor3x3_42 * invDet, -minor3x3_43 * invDet, +minor3x3_44 * invDet
	);
}

// Multiply by another matrix, in-place
void Matrix4::multiplyBy(const Matrix4& other)
{
    *this = getMultipliedBy(other);
}

// Add a translation component
void Matrix4::translateBy(const Vector3& translation)
{
    multiplyBy(getTranslation(translation));
}

// Add a scale component
void Matrix4::scaleBy(const Vector3& scale)
{
    multiplyBy(getScale(scale));
}