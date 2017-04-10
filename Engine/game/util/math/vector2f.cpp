#include "vector2f.h"

#include <math.h>

Vector2f::Vector2f(const float& inf) : Vector2f(inf, inf)
{
}

Vector2f::Vector2f(const Vector2f& copy) : Vector2f(copy.x, copy.y)
{
}

Vector2f::Vector2f(const float& x, const float& y) : x(x), y(y)
{
}

float Vector2f::GetLength() const
{
	return sqrtf(this->GetLengthSq());
}

float Vector2f::GetLengthSq() const
{
	return this->x * this->x + this->y * this->y;
}
