#pragma once

#include "../../util/math/vector3f.h"

class ModelMesh;

class Collision
{
public:
	virtual ~Collision() { }

	virtual void Set(ModelMesh *mesh) = 0;

	virtual bool Intersects(const Vector3f& point) = 0;
};
