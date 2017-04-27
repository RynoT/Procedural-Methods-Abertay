#pragma once

#include "../model.h"

class IslandModel : public Model
{
public:
	IslandModel();

	virtual bool Initialize(ID3D11Device *device) override;

	inline const int& GetLastSurfaceIndex() const { return this->m_SurfaceLastIndex; }

private:
	int m_SurfaceLastIndex;
};
