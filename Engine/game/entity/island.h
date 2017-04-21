#pragma once

#include "model_entity.h"

class Island : public ModelEntity
{
public:
	Island(ID3D11Device *device);

	bool GetSurfaceY(float x, float z, float& out) const;
};
