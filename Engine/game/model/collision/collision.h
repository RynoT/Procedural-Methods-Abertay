#pragma once

#include "../../util/math/vector3f.h"

class ModelMesh;

class Collision
{
public:
	virtual ~Collision() { }

	// Set this collision to a mesh. Derived classes will handle this uniquely
	virtual void Set(ModelMesh *mesh) = 0;

	// Whether or not this collision intersects this point
	virtual bool Intersects(const Vector3f& point) = 0;
};
