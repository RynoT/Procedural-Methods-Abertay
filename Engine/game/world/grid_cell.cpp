#include "grid_cell.h"

#include <cstdlib>

#include "../../d3dclass.h"
#include "../../timerclass.h"
#include "../entity/model_entity.h"

#define MIN_ISLAND_RADIUS_PERC 0.1f
#define MAX_ISLAND_RADIUS_PERC 0.45f

#define MIN_ISLAND_RADIUS (size * MIN_ISLAND_RADIUS_PERC)
#define MAX_ISLAND_RADIUS (size * MAX_ISLAND_RADIUS_PERC)

#define pseudo_seed(x, y) srand(TimerClass::SeedOffset + (x << 16 | y))
#define pseudo_random() (rand() / float(RAND_MAX)) // Random number between 0-1

GridCell::GridCell() : m_CellX(0), m_CellY(0), m_Model(nullptr)
{
}

void GridCell::GenerateIsland(D3DClass *d3d, const int& x, const int& y, const float& size)
{
	if (this->m_Model != nullptr)
	{
		delete this->m_Model;
		this->m_Model = nullptr;
	}

	// We must seed the random using x and y so that our random numbers are always consistent
	pseudo_seed(x, y);

	// Generate random radius for island
	float radius = MIN_ISLAND_RADIUS + (MAX_ISLAND_RADIUS - MIN_ISLAND_RADIUS) * pseudo_random();

	// Generate relative position within grid according to size and radius
	float rx = radius + (size - radius * 2.0f) * pseudo_random(), ry = radius + (size - radius * 2.0f) * pseudo_random();

	this->m_Model = new ModelEntity;
	this->m_Model->SetFrom(d3d->GetDevice(), "../../data/cube.txt", L"../../data/seafloor.dds");
	this->m_Model->SetRenderMethod([](ModelEntity *entity) {});
}
