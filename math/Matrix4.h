#pragma once

/// \file
/// \brief Matrix data types and related operations.

#include "Vector3.h"
#include "Vector4.h"
#include "pi.h"

class Quaternion;

/**
 * A 4x4 matrix stored in double-precision floating-point.
 *
 * The elements of this matrix are stored columnwise in memory:
 *
 * |  0    4    8   12 |
 * |  1    5    9   13 |
 * |  2    6   10   14 |
 * |  3    7   11   15 |
 *
 * or, alternatively, as the 4 columns are regarded as 4 vectors named x, y, z, t:
 *
 * | xx   yx   zx   tx |
 * | xy   yy   zy   ty |
 * | xz   yz   zz   tz |
 * | xw   yw   zw   tw |
 */
class Matrix4
{
    // Elements of the 4x4 matrix. These appear to be treated COLUMNWISE, i.e.
    // elements [0] through [3] are the first column, [4] through [7] are the
    // second column, etc.
    double _m[16];

private:

    // Initialising constructor, elements are passed in column-wise order
    Matrix4(double xx_, double xy_, double xz_, double xw_,
            double yx_, double yy_, double yz_, double yw_,
            double zx_, double zy_, double zz_, double zw_,
            double tx_, double ty_, double tz_, double tw_);

    // Construct a pure-rotation matrix about the X axis from sin and cosine of
    // an angle in radians
    static Matrix4 getRotationAboutXForSinCos(double s, double c);

    // Construct a pure-rotation matrix about the Y axis from sin and cosine of
    // an angle in radians
    static Matrix4 getRotationAboutYForSinCos(double s, double c);

    // Construct a pure-rotation matrix about the Z axis from sin and cosine of
    // an angle in radians
    static Matrix4 getRotationAboutZForSinCos(double s, double c);

public:

    /// Construct a matrix with uninitialised values.
    Matrix4() { }

    /// Obtain the identity matrix.
    static const Matrix4& getIdentity();

    /// Get a matrix representing the given 3D translation.
    static Matrix4 getTranslation(const Vector3& translation);

    /**
     * \brief Construct a rotation from one vector onto another vector.
     *
     * The rotational axis is defined by the normalised cross product of those
     * two vectors, the angle can be retrieved from the dot product.
     */
    static Matrix4 getRotation(const Vector3& a, const Vector3& b);

    /**
     * greebo: Returns the rotation matrix defined by an arbitrary axis
     * and an angle.
     *
     * Important: the axis vector must be normalised.
     */
    static Matrix4 getRotation(const Vector3& axis, const double angle);

    /**
     * Constructs a pure-rotation matrix from the given quaternion.
     */
    static Matrix4 getRotation(const Quaternion& quaternion);

    /**
     * Constructs a pure-rotation matrix from the given quaternion, quantised.
     */
    static Matrix4 getRotationQuantised(const Quaternion& quaternion);

    /// Construct a rotation matrix about the Z axis for a given angle
    template<typename Unit_T> static Matrix4 getRotationAboutZ(Unit_T angle)
    {
        double radians = angle.asRadians();
        return getRotationAboutZForSinCos(sin(radians), cos(radians));
    }

    /**
     * Constructs a pure-rotation matrix from a set of euler angles (radians) in the order (x, y, z).
     */
    static Matrix4 getRotationForEulerXYZ(const Vector3& euler);

    /**
     * Constructs a pure-rotation matrix from a set of euler angles (degrees) in the order (x, y, z).
     */
    static Matrix4 getRotationForEulerXYZDegrees(const Vector3& euler);

    /**
     * \brief
     * Get a matrix representing the given scale in 3D space.
     *
     * \param scale
     * Vector3 representing the scale.
     */
    static Matrix4 getScale(const Vector3& scale);

    /**
     * \brief
     * Construct a matrix containing the given elements.
     *
     * The elements are specified column-wise, starting with the left-most
     * column.
     */
    static Matrix4 byColumns(double xx, double xy, double xz, double xw,
                             double yx, double yy, double yz, double yw,
                             double zx, double zy, double zz, double zw,
                             double tx, double ty, double tz, double tw);

    /**
     * \brief
     * Construct a matrix containing the given elements.
     *
     * The elements are specified row-wise, starting with the top row.
     */
    static Matrix4 byRows(double xx, double yx, double zx, double tx,
                          double xy, double yy, double zy, double ty,
                          double xz, double yz, double zz, double tz,
                          double xw, double yw, double zw, double tw);

    enum Handedness
    {
        RIGHTHANDED = 0,
        LEFTHANDED = 1,
    };

    /**
     * Return matrix elements
     * \{
     */
    double& xx()             { return _m[0]; }
    const double& xx() const { return _m[0]; }
    double& xy()             { return _m[1]; }
    const double& xy() const { return _m[1]; }
    double& xz()             { return _m[2]; }
    const double& xz() const { return _m[2]; }
    double& xw()             { return _m[3]; }
    const double& xw() const { return _m[3]; }
    double& yx()             { return _m[4]; }
    const double& yx() const { return _m[4]; }
    double& yy()             { return _m[5]; }
    const double& yy() const { return _m[5]; }
    double& yz()             { return _m[6]; }
    const double& yz() const { return _m[6]; }
    double& yw()             { return _m[7]; }
    const double& yw() const { return _m[7]; }
    double& zx()             { return _m[8]; }
    const double& zx() const { return _m[8]; }
    double& zy()             { return _m[9]; }
    const double& zy() const { return _m[9]; }
    double& zz()             { return _m[10]; }
    const double& zz() const { return _m[10]; }
    double& zw()             { return _m[11]; }
    const double& zw() const { return _m[11]; }
    double& tx()             { return _m[12]; }
    const double& tx() const { return _m[12]; }
    double& ty()             { return _m[13]; }
    const double& ty() const { return _m[13]; }
    double& tz()             { return _m[14]; }
    const double& tz() const { return _m[14]; }
    double& tw()             { return _m[15]; }
    const double& tw() const { return _m[15]; }
    /**
     * \}
     */

    /**
     * Return columns of the matrix as vectors.
     * \{
     */
    Vector4& xCol()
    {
        return reinterpret_cast<Vector4&>(xx());
    }
    const Vector4& xCol() const
    {
        return reinterpret_cast<const Vector4&>(xx());
    }
    Vector4& yCol()
    {
        return reinterpret_cast<Vector4&>(yx());
    }
    const Vector4& yCol() const
    {
        return reinterpret_cast<const Vector4&>(yx());
    }
    Vector4& zCol()
    {
        return reinterpret_cast<Vector4&>(zx());
    }
    const Vector4& zCol() const
    {
        return reinterpret_cast<const Vector4&>(zx());
    }
    Vector4& tCol()
    {
        return reinterpret_cast<Vector4&>(tx());
    }
    const Vector4& tCol() const
    {
        return reinterpret_cast<const Vector4&>(tx());
    }
    /**
     * \}
     */

    /**
     * Cast to double* for use with GL functions that accept a double
     * array, also provides operator[].
     */
    operator double* ()
    {
        return _m;
    }

    /**
     * Cast to const double* to provide operator[] for const objects.
     */
    operator const double* () const
    {
        return _m;
    }

    /// Transpose this matrix in-place.
    void transpose();

    /// Return a transposed copy of this matrix.
    Matrix4 getTransposed() const;

    /// Return the affine inverse of this transformation matrix.
    Matrix4 getInverse() const;

    /// Affine invert this matrix in-place.
    void invert()
    {
        *this = getInverse();
    }

    /// Return the full inverse of this matrix.
    Matrix4 getFullInverse() const;

    /// Invert this matrix in-place.
    void invertFull()
    {
        *this = getFullInverse();
    }

    /**
     * \brief
     * Returns the given 3-component point transformed by this matrix.
     *
     * The point is assumed to have a W component of 1.
     */
    template<typename Element>
    BasicVector3<Element> transformPoint(const BasicVector3<Element>& point) const;

    /**
     * Returns the given 3-component direction transformed by this matrix.
     * The given vector is treated as direction so it won't receive a translation, just like
     * a 4-component vector with its w-component set to 0 would be transformed.
     */
    template<typename Element>
    BasicVector3<Element> transformDirection(const BasicVector3<Element>& direction) const;

    /**
     * \brief Use this matrix to transform the provided vector and return a new
     * vector containing the result.
     *
     * \param vector4
     * The 4-element vector to transform.
     */
    template<typename Element>
    BasicVector4<Element> transform(const BasicVector4<Element>& vector4) const;

    /**
     * \brief
     * Return the result of this matrix post-multiplied by another matrix.
     */
    Matrix4 getMultipliedBy(const Matrix4& other) const;

    /**
     * \brief
     * Post-multiply this matrix by another matrix, in-place.
     */
    void multiplyBy(const Matrix4& other);

    /// Returns this matrix pre-multiplied by the other
    Matrix4 getPremultipliedBy(const Matrix4& other) const
    {
        return other.getMultipliedBy(*this);
    }

    /// Pre-multiplies this matrix by other in-place.
    void premultiplyBy(const Matrix4& other)
    {
        *this = getPremultipliedBy(other);
    }

    /**
     * \brief
     * Add a translation component to the transformation represented by this
     * matrix.
     *
     * Equivalent to multiplyBy(Matrix4::getTranslation(translation));
     */
    void translateBy(const Vector3& translation);

    /**
     * \brief Add a translation component to the transformation represented by
     * this matrix.
     *
     * Equivalent to getMultipliedBy(Matrix4::getTranslation(translation));
     */
    Matrix4 getTranslatedBy(const Vector3& translation) const
    {
        return getMultipliedBy(Matrix4::getTranslation(translation));
    }

    /**
     * \brief
     * Add a scale component to the transformation represented by this matrix.
     *
     * Equivalent to multiplyBy(Matrix4::getScale(scale));
     */
    void scaleBy(const Vector3& scale);

    /**
     * \brief
     * Add a pivoted scale transformation to this matrix.
     */
    void scaleBy(const Vector3& scale, const Vector3& pivot);

    /**
     * Returns true if this and the given matrix are exactly element-wise equal.
     * This and the other matrix must be affine.
     */
    bool isAffineEqual(const Matrix4& other) const;

    /**
     * Returns RIGHTHANDED if this is right-handed, else returns LEFTHANDED.
     */
    Handedness getHandedness() const;

    /// Return the 3-element translation component of this matrix
    const Vector3& translation() const
    {
        return tCol().getVector3();
    }

    /**
     * Concatenates this with the rotation transform produced
     * by euler angles (degrees) in the order (x, y, z).
     * The concatenated rotation occurs before self.
     */
    void rotateByEulerXYZDegrees(const Vector3& euler)
    {
        multiplyBy(getRotationForEulerXYZDegrees(euler));
    }

    /**
     * Calculates and returns a set of euler angles in radians that produce
     * the rotation component of this matrix when applied in the order (x, y, z).
     * This matrix must be affine and orthonormal (unscaled) to produce a meaningful result.
     */
    Vector3 getEulerAnglesXYZ() const;

    /**
     * Calculates and returns a set of euler angles in degrees that produce
     * the rotation component of this matrix when applied in the order (x, y, z).
     * This matrix must be affine and orthonormal (unscaled) to produce a meaningful result.
     */
    Vector3 getEulerAnglesXYZDegrees() const;

    /**
     * Calculates and returns the (x, y, z) scale values that produce the scale component of this matrix.
     * This matrix must be affine and orthogonal to produce a meaningful result.
     */
    Vector3 getScale() const;
};

// ===========================================================================
// Operators
// ===========================================================================

/// Multiply two matrices together
inline Matrix4 operator* (const Matrix4& m1, const Matrix4& m2)
{
    return m1.getMultipliedBy(m2);
}

/// Subtract two matrices
inline Matrix4 operator- (const Matrix4& l, const Matrix4& r)
{
    return Matrix4::byColumns(
        l.xx() - r.xx(), l.xy() - r.xy(), l.xz() - r.xz(), l.xw() - r.xw(),
        l.yx() - r.yx(), l.yy() - r.yy(), l.yz() - r.yz(), l.yw() - r.yw(),
        l.zx() - r.zx(), l.zy() - r.zy(), l.zz() - r.zz(), l.zw() - r.zw(),
        l.tx() - r.tx(), l.ty() - r.ty(), l.tz() - r.tz(), l.tw() - r.tw()
    );
}

/**
 * \brief Multiply a 4-component vector by this matrix.
 *
 * Equivalent to m.transform(v).
 */
template<typename T>
BasicVector4<T> operator* (const Matrix4& m, const BasicVector4<T>& v)
{
    return m.transform(v);
}

/**
 * \brief Multiply a 3-component vector by this matrix.
 *
 * The vector is upgraded to a 4-component vector with a W component of 1, i.e.
 * equivalent to m.transformPoint(v).
 */
template<typename T>
BasicVector3<T> operator* (const Matrix4& m, const BasicVector3<T>& v)
{
    return m.transformPoint(v);
}

// =========================================================================================
// Inlined member definitions
// =========================================================================================

// Construct a matrix with given column elements
inline Matrix4 Matrix4::byColumns(double xx, double xy, double xz, double xw,
                                  double yx, double yy, double yz, double yw,
                                  double zx, double zy, double zz, double zw,
                                  double tx, double ty, double tz, double tw)
{
    return Matrix4(xx, xy, xz, xw,
                   yx, yy, yz, yw,
                   zx, zy, zz, zw,
                   tx, ty, tz, tw);
}

// Construct a matrix with given row elements
inline Matrix4 Matrix4::byRows(double xx, double yx, double zx, double tx,
                               double xy, double yy, double zy, double ty,
                               double xz, double yz, double zz, double tz,
                               double xw, double yw, double zw, double tw)
{
    return Matrix4(xx, xy, xz, xw,
                   yx, yy, yz, yw,
                   zx, zy, zz, zw,
                   tx, ty, tz, tw);
}

// Post-multiply this with other
inline Matrix4 Matrix4::getMultipliedBy(const Matrix4& other) const
{
    return Matrix4::byColumns(
        other[0] * _m[0] + other[1] * _m[4] + other[2] * _m[8] + other[3] * _m[12],
        other[0] * _m[1] + other[1] * _m[5] + other[2] * _m[9] + other[3] * _m[13],
        other[0] * _m[2] + other[1] * _m[6] + other[2] * _m[10]+ other[3] * _m[14],
        other[0] * _m[3] + other[1] * _m[7] + other[2] * _m[11]+ other[3] * _m[15],
        other[4] * _m[0] + other[5] * _m[4] + other[6] * _m[8] + other[7] * _m[12],
        other[4] * _m[1] + other[5] * _m[5] + other[6] * _m[9] + other[7] * _m[13],
        other[4] * _m[2] + other[5] * _m[6] + other[6] * _m[10]+ other[7] * _m[14],
        other[4] * _m[3] + other[5] * _m[7] + other[6] * _m[11]+ other[7] * _m[15],
        other[8] * _m[0] + other[9] * _m[4] + other[10]* _m[8] + other[11]* _m[12],
        other[8] * _m[1] + other[9] * _m[5] + other[10]* _m[9] + other[11]* _m[13],
        other[8] * _m[2] + other[9] * _m[6] + other[10]* _m[10]+ other[11]* _m[14],
        other[8] * _m[3] + other[9] * _m[7] + other[10]* _m[11]+ other[11]* _m[15],
        other[12]* _m[0] + other[13]* _m[4] + other[14]* _m[8] + other[15]* _m[12],
        other[12]* _m[1] + other[13]* _m[5] + other[14]* _m[9] + other[15]* _m[13],
        other[12]* _m[2] + other[13]* _m[6] + other[14]* _m[10]+ other[15]* _m[14],
        other[12]* _m[3] + other[13]* _m[7] + other[14]* _m[11]+ other[15]* _m[15]
    );
}

/// Compare two matrices elementwise for equality
inline bool operator==(const Matrix4& l, const Matrix4& r)
{
    return l.xx() == r.xx() && l.xy() == r.xy() && l.xz() == r.xz() && l.xw() == r.xw()
        && l.yx() == r.yx() && l.yy() == r.yy() && l.yz() == r.yz() && l.yw() == r.yw()
        && l.zx() == r.zx() && l.zy() == r.zy() && l.zz() == r.zz() && l.zw() == r.zw()
        && l.tx() == r.tx() && l.ty() == r.ty() && l.tz() == r.tz() && l.tw() == r.tw();
}

/// Compare two matrices elementwise for inequality
inline bool operator!=(const Matrix4& l, const Matrix4& r)
{
    return !(l == r);
}

inline bool Matrix4::isAffineEqual(const Matrix4& other) const
{
    return xx() == other.xx() &&
            xy() == other.xy() &&
            xz() == other.xz() &&
            yx() == other.yx() &&
            yy() == other.yy() &&
            yz() == other.yz() &&
            zx() == other.zx() &&
            zy() == other.zy() &&
            zz() == other.zz() &&
            tx() == other.tx() &&
            ty() == other.ty() &&
            tz() == other.tz();
}

inline Matrix4::Handedness Matrix4::getHandedness() const
{
    return (xCol().getVector3().crossProduct(yCol().getVector3()).dot(zCol().getVector3()) < 0.0f) ? LEFTHANDED : RIGHTHANDED;
}

template<typename Element>
BasicVector3<Element> Matrix4::transformPoint(const BasicVector3<Element>& point) const
{
    return BasicVector3<Element>(
        static_cast<Element>(xx() * point[0] + yx() * point[1] + zx() * point[2] + tx()),
        static_cast<Element>(xy() * point[0] + yy() * point[1] + zy() * point[2] + ty()),
        static_cast<Element>(xz() * point[0] + yz() * point[1] + zz() * point[2] + tz())
    );
}

template<typename Element>
BasicVector3<Element> Matrix4::transformDirection(const BasicVector3<Element>& direction) const
{
    return BasicVector3<Element>(
        static_cast<Element>(xx() * direction[0] + yx() * direction[1] + zx() * direction[2]),
        static_cast<Element>(xy() * direction[0] + yy() * direction[1] + zy() * direction[2]),
        static_cast<Element>(xz() * direction[0] + yz() * direction[1] + zz() * direction[2])
    );
}

template<typename Element>
BasicVector4<Element> Matrix4::transform(const BasicVector4<Element>& vector4) const
{
    return BasicVector4<Element>(
        static_cast<Element>(_m[0] * vector4[0] + _m[4] * vector4[1] + _m[8]  * vector4[2] + _m[12] * vector4[3]),
        static_cast<Element>(_m[1] * vector4[0] + _m[5] * vector4[1] + _m[9]  * vector4[2] + _m[13] * vector4[3]),
        static_cast<Element>(_m[2] * vector4[0] + _m[6] * vector4[1] + _m[10] * vector4[2] + _m[14] * vector4[3]),
        static_cast<Element>(_m[3] * vector4[0] + _m[7] * vector4[1] + _m[11] * vector4[2] + _m[15] * vector4[3])
    );
}

inline Vector3 Matrix4::getEulerAnglesXYZ() const
{
    double a = asin(-xz());
    double ca = cos(a);

    if (fabs(ca) > 0.005f) // Gimbal lock?
    {
        return Vector3(
            atan2(yz() / ca, zz() / ca),
            a,
            atan2(xy() / ca, xx() / ca)
        );
    }
    else // Gimbal lock has occurred
    {
        return Vector3(
            atan2(-zy(), yy()),
            a,
            0
        );
    }
}

inline Vector3 Matrix4::getEulerAnglesXYZDegrees() const
{
    Vector3 eulerRad = getEulerAnglesXYZ();
    return Vector3(radians_to_degrees(eulerRad.x()), radians_to_degrees(eulerRad.y()), radians_to_degrees(eulerRad.z()));
}

inline Vector3 Matrix4::getScale() const
{
    return Vector3(
        xCol().getVector3().getLength(),
        yCol().getVector3().getLength(),
        zCol().getVector3().getLength()
    );
}

inline void Matrix4::scaleBy(const Vector3& scale, const Vector3& pivot)
{
    translateBy(pivot);
    scaleBy(scale);
    translateBy(-pivot);
}

/// Debug stream insertion operator for Matrix4
inline std::ostream& operator<<(std::ostream& st, const Matrix4& m)
{
    st << "[" << m[0] << " " << m[4] << " " << m[8] << " " << m[12] << "; ";
    st << m[1] << " " << m[5] << " " << m[9] << " " << m[13] << "; ";
    st << m[2] << " " << m[6] << " " << m[10] << " " << m[14] << "; ";
    st << m[3] << " " << m[7] << " " << m[11] << " " << m[15] << "]";
    return st;
}
