#pragma once

#include "../model.h"

class Path : public Model
{
public:
	Path();

	virtual bool Initialize(ID3D11Device* device) override;
};
