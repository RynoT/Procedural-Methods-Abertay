#pragma once

#include "../model.h"

// This model is present when the player hovers over an island
class IslandHoverModel : public Model
{
public:
	virtual bool Initialize(ID3D11Device* device) override;
};
