#include "path.h"

Path::Path(const Vector3f& pointA, const Vector3f& pointB) : m_PointA(pointA), m_PointB(pointB)
{
	Vector3f vector = pointB - pointA;
	Entity::SetScale(vector.x, vector.y, vector.z);
}
