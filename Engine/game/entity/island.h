#pragma once

#include "model_entity.h"

#define ISLAND_TYPE_COUNT 3 // How many items are in the enum below

enum IslandType
{
	PERLIN, FAULT, FLAT
};

class Island : public ModelEntity
{
public:
	Island(ID3D11Device *device, const IslandType& type);

	// Get the y coordinate from the given x and z (world position)
	bool GetSurfaceY(float x, float z, float& out) const;

	inline const IslandType& GetType() const { return this->m_Type; }

	// Get a random type from the given parameter. The parameter should be a value from 0-1.
	inline static IslandType GetType(const float& range)
	{
		return IslandType(int(ISLAND_TYPE_COUNT * max(0.0f, min(1.0f, range))));
	}

private:
	IslandType m_Type;
};
