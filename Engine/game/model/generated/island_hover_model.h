#pragma once

#include "../model.h"

#include "../../util/math/vector3f.h"

class IslandHoverModel : public Model
{
public:
	virtual bool Initialize(ID3D11Device* device) override;

};
