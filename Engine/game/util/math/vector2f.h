#pragma once

#include "../../../d3dclass.h"

#define VECTOR2_SPLIT(vector) (vector).x, (vector).y

struct Vector2f
{
	explicit Vector2f(const float& inf = 0.0f);
	Vector2f(const Vector2f& copy);
	Vector2f(const float& x, const float& y);
	virtual ~Vector2f() { }

	virtual float GetLength() const;

	virtual float GetLengthSq() const;

	inline Vector2f& add(const float& x, const float& y) { this->x += x; this->y += y; return *this; }

	inline Vector2f& subtract(const float& x, const float& y) { this->x -= x; this->y -= y; return *this; }

	inline Vector2f& multiply(const float& scalar) { return this->multiply(scalar, scalar); }

	inline Vector2f& multiply(const float& x, const float& y) { this->x *= x; this->y *= y; return *this; }

	inline Vector2f& divide(const float& scalar) { return this->divide(scalar, scalar); }

	inline Vector2f& divide(const float& x, const float& y) { this->x /= x; this->y /= y; return *this; }

	inline Vector2f& set(const Vector2f& vector) { return this->set(vector.x, vector.y); }

	inline Vector2f& set(const float& x, const float& y) { this->x = x; this->y = y; return *this; }

	Vector2f operator+(const Vector2f& vector) const { return{ this->x + vector.x, this->y + vector.y }; }

	Vector2f& operator+=(const Vector2f& vector) { return this->add(vector.x, vector.y); }

	Vector2f operator-(const Vector2f& vector) const { return{ this->x - vector.x, this->y - vector.y }; }

	Vector2f& operator-=(const Vector2f& vector) { return this->subtract(vector.x, vector.y); }

	Vector2f operator*(const float& scalar) const { return{ this->x * scalar, this->y * scalar }; }

	Vector2f operator*(const Vector2f& vector) const { return{ this->x * vector.x, this->y * vector.y }; }

	Vector2f& operator*=(const float& scalar) { return this->multiply(scalar); }

	Vector2f& operator*=(const Vector2f& vector) { return this->multiply(vector.x, vector.y); }

	Vector2f operator/(const float& scalar) const { return{ this->x / scalar, this->y / scalar }; }

	Vector2f operator/(const Vector2f& vector) const { return{ this->x / vector.x, this->y / vector.y }; }

	Vector2f& operator/=(const float& scalar) { return this->divide(scalar); }

	Vector2f& operator/=(const Vector2f& vector) { return this->divide(vector.x, vector.y); }

	operator D3DXVECTOR2() const { return D3DXVECTOR2(this->x, this->y); }

public:
	float x, y;
};