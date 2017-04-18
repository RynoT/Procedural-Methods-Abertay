#pragma once

#include "../model.h"

class IslandModel : public Model
{
public:
	IslandModel(const float& x, const float& y);

	virtual bool Initialize(ID3D11Device *device) override;

private:
	const float m_X, m_Y;
};