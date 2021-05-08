#pragma once

#include "Core/ObjectBase.hpp"

#include <ostream>

#include "Core/BasicTypes.hpp"
#include "Core/Assert/Assert.hpp"
#include "Maths/MathUtils.hpp"

namespace DE {

class Vector2;
class Vector4;

/*!
 \brief 3D Vector.
 */
class Vector3 : public ObjectBase {

public:

	//-------------------------------------------------------------------
	// MEMBERS
	//-------------------------------------------------------------------

	f32 x, y, z;

	//-------------------------------------------------------------------
	// CONSTRUCTORS/DESTRUCTOR
	//-------------------------------------------------------------------

	GENERATE_METADATA(CONSTRUCTOR, Vector3)

	Vector3(f32 x, f32 y, f32 z);
	Vector3(const Vector3 &other);
	Vector3(const Vector2 &other);
	Vector3(const Vector2 &other, f32 z);
	Vector3(const Vector4 &other);

	//-------------------------------------------------------------------
	// METHODS
	//-------------------------------------------------------------------

	Vector3& set(const Vector3 &rhs);
	Vector3& set(f32 x, f32 y, f32 z);
	Vector3& add(const Vector3 &rhs);
	Vector3& sub(const Vector3 &rhs);
	Vector3& mul(const Vector3 &rhs);
	Vector3& div(const Vector3 &rhs);
	Vector3& add(f32 rhs);
	Vector3& sub(f32 rhs);
	Vector3& mul(f32 rhs);
	Vector3& div(f32 rhs);

	f32 len() const;
	f32 sqrlen() const;
	f32 max() const;
	f32 min() const;
	f32 dot(const Vector3 &v) const;
	Vector3& nor();
	f32 dst(const Vector3 &v) const;
	f32 sqrdst(const Vector3 &v) const;
	bool eq(const Vector3 &v, f32 e) const; // epsilon equal
	bool eq(const Vector3 &v) const; // equal
	Vector3& cross(const Vector3 &v); // only defined for 3D space
	Vector3& lerp(const Vector3 &target, f32 t);
	Vector3& nlerp(const Vector3 &target, f32 t);
	Vector3& slerp(const Vector3 &target, f32 t);
	f32 angle(const Vector3 &v, const Vector3 &n) const;
	f32 angle(const Vector3 &v) const;
	Vector3& clamp(f32 maxLength);

	//-------------------------------------------------------------------
	// OPERATORS
	//-------------------------------------------------------------------

	Vector3& operator=(const Vector3 &rhs) {
		if (this == &rhs)
			return *this; // handle self assignment
		//assignment operator
		return this->set(rhs);
	}

	Vector3& operator+=(const Vector3 &rhs) {
		// can be parallelized with SIMD auto-vectorization
		return this->add(rhs);
	}

	Vector3& operator-=(const Vector3 &rhs) {
		return this->sub(rhs);
	}

	Vector3& operator*=(const Vector3 &rhs) {
		return this->mul(rhs);
	}

	Vector3& operator/=(const Vector3 &rhs) {
		return this->div(rhs);
	}

	Vector3& operator+=(f32 rhs) {
		return this->add(rhs);
	}

	Vector3& operator-=(f32 rhs) {
		return this->sub(rhs);
	}

	Vector3& operator*=(f32 rhs) {
		return this->mul(rhs);
	}

	Vector3& operator/=(f32 rhs) {
		return this->div(rhs);
	}

	bool operator==(const Vector3 &rhs) const {
		return this->eq(rhs);
	}

	bool operator!=(const Vector3 &rhs) {
		return !((*this) == rhs);
	}

	Vector3 operator+(const Vector3 &rhs) const {
		return Vector3(*this) += rhs;
	}

	Vector3 operator-(const Vector3 &rhs) const {
		return Vector3(*this) -= rhs;
	}

	Vector3 operator-() const {
		return Vector3(*this) *= -1;
	}

	Vector3 operator*(const Vector3 &rhs) const {
		return Vector3(*this) *= rhs;
	}

	Vector3 operator/(const Vector3 &rhs) const {
		return Vector3(*this) /= rhs;
	}

	Vector3 operator+(f32 rhs) const {
		return Vector3(*this) += rhs;
	}

	Vector3 operator-(f32 rhs) const {
		return Vector3(*this) -= rhs;
	}

	Vector3 operator*(f32 rhs) const {
		return Vector3(*this) *= rhs;
	}

	Vector3 operator/(f32 rhs) const {
		return Vector3(*this) /= rhs;
	}

	// can be used for assignment
	f32& operator[](const size_t index) {
		ASSERT(index >= 0 && index < 3, "Index out of bounds.");

		return *(&x + index);
	}

	// read only
	f32 operator[](const size_t index) const {
		ASSERT(index >= 0 && index < 3, "Index out of bounds.");
		return *(&x + index);
	}

	friend std::ostream& operator<<(std::ostream &out, const Vector3 &v) {
		out << "(" << v.x << ", " << v.y << ", " << v.z << ")";
		return out;
	}
};
}

