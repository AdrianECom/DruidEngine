#pragma once

#include "Core/ObjectBase.hpp"

#include <ostream>

#include "Core/BasicTypes.hpp"
#include "Core/Assert/Assert.hpp"
#include "Maths/MathUtils.hpp"

namespace DE {

class Vector2;
class Vector3;

/*!
 \brief 4D Vector.
 */
class Vector4: public ObjectBase /*16 bytes alignment*/{

public:

	//-------------------------------------------------------------------
	// MEMBERS
	//-------------------------------------------------------------------

	f32 x, y, z, w;

	//-------------------------------------------------------------------
	// CONSTRUCTORS/DESTRUCTOR
	//-------------------------------------------------------------------

	GENERATE_METADATA(CONSTRUCTOR, Vector4)

	Vector4(f32 x, f32 y, f32 z, f32 w);
	Vector4(const Vector4 &other);
	Vector4(const Vector3 &other);
	Vector4(const Vector2 &other);
	Vector4(const Vector3 &other, f32 w);
	Vector4(const Vector2 &other, f32 z, f32 w);

	//-------------------------------------------------------------------
	// METHODS
	//-------------------------------------------------------------------

	Vector4& set(const Vector4 &rhs);
	Vector4& set(f32 x, f32 y, f32 z, f32 w);
	Vector4& add(const Vector4 &rhs);
	Vector4& sub(const Vector4 &rhs);
	Vector4& mul(const Vector4 &rhs);
	Vector4& div(const Vector4 &rhs);
	Vector4& add(f32 rhs);
	Vector4& sub(f32 rhs);
	Vector4& mul(f32 rhs);
	Vector4& div(f32 rhs);

	f32 len() const;
	f32 sqrlen() const;
	f32 max() const;
	f32 min() const;
	f32 dot(const Vector4 &v) const;
	Vector4& nor();
	f32 dst(const Vector4 &v) const;
	f32 sqrdst(const Vector4 &v) const;
	bool eq(const Vector4 &v, f32 e) const; // epsilon equal
	bool eq(const Vector4 &v) const; // equal
	Vector4& lerp(const Vector4 &target, f32 t);
	Vector4& clamp(f32 maxLength);

	//-------------------------------------------------------------------
	// OPERATORS
	//-------------------------------------------------------------------

	Vector4& operator=(const Vector4 &rhs) {
		if (this == &rhs)
			return *this; // handle self assignment
		//assignment operator
		return this->set(rhs);
	}

	Vector4& operator+=(const Vector4 &rhs) {
		// can be parallelized with SIMD auto-vectorization
		return this->add(rhs);
	}

	Vector4& operator-=(const Vector4 &rhs) {
		return this->sub(rhs);
	}

	Vector4& operator*=(const Vector4 &rhs) {
		return this->mul(rhs);
	}

	Vector4& operator/=(const Vector4 &rhs) {
		return this->div(rhs);
	}

	Vector4& operator+=(f32 rhs) {
		return this->add(rhs);
	}

	Vector4& operator-=(f32 rhs) {
		return this->sub(rhs);
	}

	Vector4& operator*=(f32 rhs) {
		return this->mul(rhs);
	}

	Vector4& operator/=(f32 rhs) {
		return this->div(rhs);
	}

	bool operator==(const Vector4 &rhs) const {
		return this->eq(rhs);
	}

	bool operator!=(const Vector4 &rhs) const {
		return !((*this) == rhs);
	}

	Vector4 operator+(const Vector4 &rhs) const {
		return Vector4(*this) += rhs;
	}

	Vector4 operator-(const Vector4 &rhs) const {
		return Vector4(*this) -= rhs;
	}

	Vector4 operator*(const Vector4 &rhs) const {
		return Vector4(*this) *= rhs;
	}

	Vector4 operator/(const Vector4 &rhs) const {
		return Vector4(*this) /= rhs;
	}

	Vector4 operator+(f32 rhs) const {
		return Vector4(*this) += rhs;
	}

	Vector4 operator-(f32 rhs) const {
		return Vector4(*this) -= rhs;
	}

	Vector4 operator-() const {
		return Vector4(*this) *= -1;
	}

	Vector4 operator*(f32 rhs) const {
		return Vector4(*this) *= rhs;
	}

	Vector4 operator/(f32 rhs) const {
		return Vector4(*this) /= rhs;
	}

	// can be used for assignment
	f32& operator[](const size_t index) {
		ASSERT(index >= 0 && index < 4, "Index out of bounds.");

		return *(&x + index);
	}

	// read only
	f32 operator[](const size_t index) const {
		ASSERT(index >= 0 && index < 4, "Index out of bounds.");
		return *(&x + index);
	}

	friend std::ostream& operator<<(std::ostream &out, const Vector4 &v) {
		out << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
		return out;
	}
};
}

