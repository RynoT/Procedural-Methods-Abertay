#include "vector3f.h"

Vector3f::Vector3f(const float& inf) : Vector3f(inf, inf, inf)
{
}

Vector3f::Vector3f(const Vector3f& copy) : Vector3f(copy.x, copy.y, copy.z)
{
}

Vector3f::Vector3f(const float& x, const float& y, const float& z) : Vector2f(x, y), z(z)
{
}

float Vector3f::GetLengthSq() const
{
	return Vector2f::GetLengthSq() + this->z * this->z;
}

Vector3f& Vector3f::normalize() 
{
	float len = this->GetLength();
	if(len != 0.0f)
	{
		return this->multiply(1.0f / len);
	}
	return *this;
}

Vector3f Vector3f::cross(const Vector3f& target) const
{
	return{
		this->y * target.z - this->z * target.y,
		-(this->x * target.z - this->z * target.x),
		this->x * target.y - this->y * target.x
	};
}
