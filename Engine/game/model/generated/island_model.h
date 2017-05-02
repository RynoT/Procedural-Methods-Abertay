#pragma once

#include <vector>

#include "../model.h"

enum IslandType;

class IslandModel : public Model
{
public:
	IslandModel(IslandType *type);

	virtual bool Initialize(ID3D11Device *device) override;

	inline const int& GetLastSurfaceIndex() const { return this->m_SurfaceLastIndex; }

protected:
	// Add noise to the island mesh data
	void AddNoise(std::vector<ModelData>& data) const;

private:
	IslandType *m_Type;
	int m_SurfaceLastIndex, m_SurfaceOuterLastIndex, m_BelowUpperLastIndex;
};
