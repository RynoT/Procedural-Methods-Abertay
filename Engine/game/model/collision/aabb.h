#pragma once

#include "collision.h"

class ModelMesh;

class CollisionAABB : public Collision
{
public:
	CollisionAABB();

	virtual void Set(ModelMesh *mesh) override;

	virtual bool Intersects(const Vector3f& point) override;

	inline const Vector3f& GetMax() const { return this->m_Max; }

	inline const Vector3f& GetMin() const { return this->m_Min; }

private:
	Vector3f m_Max, m_Min;
};
