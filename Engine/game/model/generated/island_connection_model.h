#pragma once

#include "../model.h"

class IslandConnectionModel : public Model
{
public:
	virtual bool Initialize(ID3D11Device *device) override;
};