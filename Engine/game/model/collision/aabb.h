#pragma once

#include "collision.h"

class ModelMesh;

class CollisionAABB : public Collision
{
public:
	CollisionAABB();

	virtual void Set(ModelMesh *mesh) override;

	virtual bool Intersects(const Vector3f& point) override;

private:
	Vector3f m_Max, m_Min;
};
