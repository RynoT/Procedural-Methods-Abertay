#pragma once

#include "model_entity.h"

class Path : public ModelEntity
{
public:
	Path(const Vector3f& pointA, const Vector3f& pointB);

	inline const Vector3f& GetPointA() const { return this->m_PointA; }

	inline const Vector3f& GetPointB() const { return this->m_PointB; }

private:
	Vector3f m_PointA, m_PointB;
};
