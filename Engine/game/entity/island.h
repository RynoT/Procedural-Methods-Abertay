#pragma once

#include "model_entity.h"

class Island : public ModelEntity
{
public:
	Island(ID3D11Device *device);

	float GetSurfaceY(float x, float z) const;
};
