#pragma once

#include "../model.h"

class QuadModel : public Model
{
public:
	virtual bool Initialize(ID3D11Device *device) override;
};
