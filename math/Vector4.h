#pragma once

#include <sstream>

/* greebo: This file contains the templated class definition of the three-component vector
 *
 * BasicVector4: A vector with three components of type <Element>
 *
 * The BasicVector4 is equipped with the most important operators like *, *= and so on.
 *
 * Note: The most commonly used Vector4 is a BasicVector4<float>, this is also defined in this file
 *
 * Note: that the multiplication of a Vector4 with another one (Vector4*Vector4) does NOT
 * result in an inner product but in a component-wise scaling. Use the .dot() method to
 * execute an inner product of two vectors.
 */

#include "lrint.h"
#include "FloatTools.h"
#include "Vector3.h"

/// A 4-element vector of type <Element>
template<typename Element>
class BasicVector4
{
    // The components of this vector
    Element _v[4];

public:
    // Public typedef to read the type of our elements
    typedef Element ElementType;

    // Constructor (no arguments)
    BasicVector4() {
        _v[0] = 0;
        _v[1] = 0;
        _v[2] = 0;
        _v[3] = 0;
    }

    /**
     * \brief Construct a BasicVector4 out of 4 explicit values.
     *
     * If the W coordinate is unspecified it will default to 1.
     */
    BasicVector4(Element x_, Element y_, Element z_, Element w_ = 1)
    {
        _v[0] = x_;
        _v[1] = y_;
        _v[2] = z_;
        _v[3] = w_;
    }

    // Construct a BasicVector4 out of a Vector3 plus a W value (default 1)
    BasicVector4(const BasicVector3<Element>& other, Element w_ = 1)
    {
        _v[0] = other.x();
        _v[1] = other.y();
        _v[2] = other.z();
        _v[3] = w_;
    }

    // Return non-constant references to the components
    Element& x() { return _v[0]; }
    Element& y() { return _v[1]; }
    Element& z() { return _v[2]; }
    Element& w() { return _v[3]; }

    // Return constant references to the components
    const Element& x() const { return _v[0]; }
    const Element& y() const { return _v[1]; }
    const Element& z() const { return _v[2]; }
    const Element& w() const { return _v[3]; }

    Element index(std::size_t i) const {
        return _v[i];
    }
    Element& index(std::size_t i) {
        return _v[i];
    }

    /**
     * \brief
     * Return a readable (pretty-printed) string representation of the vector
     *
     * We need a dedicated function for this because the standard operator<< is
     * already used for serialisation to the less readable space-separated text
     * format.
     */
    std::string pp() const
    {
        std::stringstream ss;
        ss << "(" << x() << ", " << y() << ", " << z() << ", " << w() << ")";
        return ss.str();
    }

    /** Compare this BasicVector4 against another for equality.
     */
    bool operator== (const BasicVector4& other) const {
        return (other.x() == x()
                && other.y() == y()
                && other.z() == z()
                && other.w() == w());
    }

    /** Compare this BasicVector4 against another for inequality.
     */
    bool operator!= (const BasicVector4& other) const {
        return !(*this == other);
    }

    /*  Define the addition operators + and += with any other BasicVector4 of type OtherElement
     *  The vectors are added to each other element-wise
     */
    template<typename OtherElement>
    BasicVector4<Element> operator+ (const BasicVector4<OtherElement>& other) const {
        return BasicVector4<Element>(
            _v[0] + static_cast<Element>(other.x()),
            _v[1] + static_cast<Element>(other.y()),
            _v[2] + static_cast<Element>(other.z()),
            _v[3] + static_cast<Element>(other.w())
        );
    }

    template<typename OtherElement>
	BasicVector4<Element>& operator+= (const BasicVector4<OtherElement>& other) {
        _v[0] += static_cast<Element>(other.x());
        _v[1] += static_cast<Element>(other.y());
        _v[2] += static_cast<Element>(other.z());
        _v[3] += static_cast<Element>(other.w());
		return *this;
    }

    /*  Define the substraction operators - and -= with any other BasicVector4 of type OtherElement
     *  The vectors are substracted from each other element-wise
     */
    template<typename OtherElement>
    BasicVector4<Element> operator- (const BasicVector4<OtherElement>& other) const {
        return BasicVector4<Element>(
            _v[0] - static_cast<Element>(other.x()),
            _v[1] - static_cast<Element>(other.y()),
            _v[2] - static_cast<Element>(other.z()),
            _v[3] - static_cast<Element>(other.w())
        );
    }

    template<typename OtherElement>
	BasicVector4<Element>& operator-= (const BasicVector4<OtherElement>& other) {
        _v[0] -= static_cast<Element>(other.x());
        _v[1] -= static_cast<Element>(other.y());
        _v[2] -= static_cast<Element>(other.z());
        _v[3] -= static_cast<Element>(other.w());
		return *this;
    }

    /*  Define the multiplication operators * and *= with another Vector4 of type OtherElement
     *
     *  The vectors are multiplied element-wise
     *
     *  greebo: This is mathematically kind of senseless, as this is a mixture of
     *  a dot product and scalar multiplication. It can be used to scale each
     *  vector component by a different factor, so maybe this comes in handy.
     */
    template<typename OtherElement>
    BasicVector4<Element> operator* (const BasicVector4<OtherElement>& other) const {
        return BasicVector4<Element>(
            _v[0] * static_cast<Element>(other.x()),
            _v[1] * static_cast<Element>(other.y()),
            _v[2] * static_cast<Element>(other.z()),
            _v[3] * static_cast<Element>(other.w())
        );
    }

    template<typename OtherElement>
	BasicVector4<Element>& operator*= (const BasicVector4<OtherElement>& other) {
        _v[0] *= static_cast<Element>(other.x());
        _v[1] *= static_cast<Element>(other.y());
        _v[2] *= static_cast<Element>(other.z());
        _v[3] *= static_cast<Element>(other.w());
		return *this;
    }


    /*  Define the multiplications * and *= with a scalar
     */
    template<typename OtherElement>
    BasicVector4<Element> operator* (const OtherElement& other) const {
        Element factor = static_cast<Element>(other);
        return BasicVector4<Element>(
            _v[0] * factor,
            _v[1] * factor,
            _v[2] * factor,
            _v[3] * factor
        );
    }

    template<typename OtherElement>
	BasicVector4<Element>& operator*= (const OtherElement& other) {
        Element factor = static_cast<Element>(other);
        _v[0] *= factor;
        _v[1] *= factor;
        _v[2] *= factor;
        _v[3] *= factor;
		return *this;
    }

    /*  Define the division operators / and /= with another Vector4 of type OtherElement
     *  The vectors are divided element-wise
     */
    template<typename OtherElement>
    BasicVector4<Element> operator/ (const BasicVector4<OtherElement>& other) const {
        return BasicVector4<Element>(
            _v[0] / static_cast<Element>(other.x()),
            _v[1] / static_cast<Element>(other.y()),
            _v[2] / static_cast<Element>(other.z()),
            _v[3] / static_cast<Element>(other.w())
        );
    }

    template<typename OtherElement>
	BasicVector4<Element>& operator/= (const BasicVector4<OtherElement>& other) {
        _v[0] /= static_cast<Element>(other.x());
        _v[1] /= static_cast<Element>(other.y());
        _v[2] /= static_cast<Element>(other.z());
        _v[3] /= static_cast<Element>(other.w());
		return *this;
    }

    /*  Define the scalar divisions / and /=
     */
    template<typename OtherElement>
    BasicVector4<Element> operator/ (const OtherElement& other) const {
        Element divisor = static_cast<Element>(other);
        return BasicVector4<Element>(
            _v[0] / divisor,
            _v[1] / divisor,
            _v[2] / divisor,
            _v[3] / divisor
        );
    }

    template<typename OtherElement>
	BasicVector4<Element>& operator/= (const OtherElement& other) {
        Element divisor = static_cast<Element>(other);
        _v[0] /= divisor;
        _v[1] /= divisor;
        _v[2] /= divisor;
        _v[3] /= divisor;
		return *this;
    }

    /* Scalar product this vector with another Vector4,
     * returning the projection of <self> onto <other>
     *
     * @param other
     * The Vector4 to dot-product with this Vector4.
     *
     * @returns
     * The inner product (a scalar): a[0]*b[0] + a[1]*b[1] + a[2]*b[2] + a[3]*b[3]
     */

    template<typename OtherT>
    Element dot(const BasicVector4<OtherT>& other) const {
        return  Element(_v[0] * other.x() +
                        _v[1] * other.y() +
                        _v[2] * other.z() +
                        _v[3] * other.w());
    }

    /** Project this homogeneous Vector4 into a Cartesian Vector3
     * by dividing by w.
     *
     * @returns
     * A Vector3 representing the Cartesian equivalent of this
     * homogeneous vector.
     */
    BasicVector3<Element> getProjected() {
        return BasicVector3<Element>(
            _v[0] / _v[3],
            _v[1] / _v[3],
            _v[2] / _v[3]);
    }

    /** Implicit cast to C-style array. This allows a Vector4 to be
     * passed directly to GL functions that expect an array (e.g.
     * glFloat4dv()). These functions implicitly provide operator[]
     * as well, since the C-style array provides this function.
     */

    operator const Element* () const {
        return _v;
    }

    operator Element* () {
        return _v;
    }

    /*  Cast this Vector4 onto a Vector3, both const and non-const
     */
    BasicVector3<Element>& getVector3() {
        return *reinterpret_cast<BasicVector3<Element>*>(_v);
    }

    const BasicVector3<Element>& getVector3() const {
        return *reinterpret_cast<const BasicVector3<Element>*>(_v);
    }

    /**
     * Equality check with tolerance epsilon.
     */
    template<typename OtherElement>
    bool isEqual(const BasicVector4<OtherElement>& other, Element epsilon) const
    {
        return float_equal_epsilon(x(), other.x(), epsilon) &&
               float_equal_epsilon(y(), other.y(), epsilon) &&
               float_equal_epsilon(z(), other.z(), epsilon) &&
               float_equal_epsilon(w(), other.w(), epsilon);
    }

}; // BasicVector4

/// Stream insertion for BasicVector4
template<typename T>
inline std::ostream& operator<<(std::ostream& st, BasicVector4<T> vec)
{
    return st << vec.x() << " " << vec.y() << " " << vec.z() << " " << vec.w();
}

/// Stream extraction for BasicVector4
template<typename T>
inline std::istream& operator>>(std::istream& st, BasicVector4<T>& vec)
{
    return st >> std::skipws >> vec.x() >> vec.y() >> vec.z() >> vec.w();
}

// A 4-element vector stored in double-precision floating-point.
typedef BasicVector4<double> Vector4;

// A 4-element vector stored in single-precision floating-point.
typedef BasicVector4<float> Vector4f;
