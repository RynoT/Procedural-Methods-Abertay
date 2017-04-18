#pragma once

#include "vector2f.h"

#define VECTOR3_SPLIT(vector) (vector).x, (vector).y, (vector).z

struct Vector3f : public Vector2f
{
	explicit Vector3f(const float& inf = 0.0f);
	Vector3f(const Vector3f& copy);
	Vector3f(const float& x, const float& y, const float& z);
	virtual ~Vector3f(){ }

	inline float GetLengthSq() const override;

	Vector3f& normalize();

	Vector3f cross(const Vector3f& target) const;

	inline Vector3f& add(const float& x, const float& y, const float& z) { Vector2f::add(x, y); this->z += z; return *this; }

	inline Vector3f& subtract(const float& x, const float& y, const float& z) { Vector2f::subtract(x, y); this->z -= z; return *this; }

	inline Vector3f& multiply(const float& scalar) { return this->multiply(scalar, scalar, scalar); }

	inline Vector3f& multiply(const float& x, const float& y, const float& z) { Vector2f::multiply(x, y); this->z *= z; return *this; }

	inline Vector3f& divide(const float& scalar) { return this->divide(scalar, scalar, scalar); }

	inline Vector3f& divide(const float& x, const float& y, const float& z) { Vector2f::divide(x, y); this->z /= z; return *this; }

	inline Vector3f& set(const Vector3f& vector) { return this->set(vector.x, vector.y, vector.z); }

	inline Vector3f& set(const float& x, const float& y, const float& z) { Vector2f::x = x; Vector2f::y = y; this->z = z; return *this; }

	Vector3f operator+(const Vector3f& vector) const { return{ Vector2f::x + vector.x, Vector2f::y + vector.y, this->z + vector.z }; }

	Vector3f& operator+=(const Vector3f& vector) { return this->add(vector.x, vector.y, vector.z); }

	Vector3f operator-(const Vector3f& vector) const { return{ Vector2f::x - vector.x, Vector2f::y - vector.y, this->z - vector.z }; }

	Vector3f& operator-=(const Vector3f& vector) { return this->subtract(vector.x, vector.y, vector.z); }

	Vector3f operator*(const float& scalar) const { return{ Vector2f::x * scalar, Vector2f::y * scalar, this->z *scalar }; }

	Vector3f operator*(const Vector3f& vector) const { return{ Vector2f::x * vector.x, Vector2f::y * vector.y, this->z * vector.z }; }

	Vector3f& operator*=(const float& scalar) { return this->multiply(scalar); }

	Vector3f& operator*=(const Vector3f& vector) { return this->multiply(vector.x, vector.y, vector.z); }

	Vector3f operator/(const float& scalar) const { return{ Vector2f::x / scalar, Vector2f::y / scalar, this->z / scalar }; }

	Vector3f operator/(const Vector3f& vector) const { return{ Vector2f::x / vector.x, Vector2f::y / vector.y, this->z / vector.z }; }

	Vector3f& operator/=(const float& scalar) { return this->divide(scalar); }

	Vector3f& operator/=(const Vector3f& vector) { return this->divide(vector.x, vector.y, vector.z); }

	operator D3DXVECTOR3() const { return D3DXVECTOR3(Vector2f::x, Vector2f::y, this->z); }

	operator D3DXVECTOR4() const { return D3DXVECTOR4(Vector2f::x, Vector2f::y, this->z, 1.0f); }

public:
	float z;
};
