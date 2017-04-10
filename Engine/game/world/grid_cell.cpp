#include "grid_cell.h"

#include <cstdlib>

#include "../../d3dclass.h"
#include "../../timerclass.h"
#include "../../modelclass.h"
#include "../entity/model_entity.h"
#include "../../textureshaderclass.h"

#define MIN_ISLAND_RADIUS_PERC 0.1f
#define MAX_ISLAND_RADIUS_PERC 0.45f

#define MIN_ISLAND_RADIUS (size * MIN_ISLAND_RADIUS_PERC)
#define MAX_ISLAND_RADIUS (size * MAX_ISLAND_RADIUS_PERC)

#define pseudo_seed(x, y) srand(TimerClass::SeedOffset + (x << 16 | y))
#define pseudo_random() (rand() / float(RAND_MAX)) // Random number between 0-1

GridCell::GridCell() : m_CellX(0), m_CellY(0), m_Model(nullptr), m_Shader(nullptr)
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
	this->m_Model->SetFrom(d3d->GetDevice(), "data/models/cube.txt", L"data/models/seafloor.dds");
	this->m_Model->SetRenderMethod([this](D3DClass* direct, const D3DXMATRIX& projection,
		const D3DXMATRIX& view, const D3DXMATRIX& model)->void { this->Render(direct, projection, view, model); });
}

void GridCell::Render(D3DClass* direct, const D3DXMATRIX& projection, const D3DXMATRIX& view, const D3DXMATRIX& model) const
{
	if(this->m_Shader == nullptr)
	{
		return;
	}
	ModelClass *modelClass = this->m_Model->GetInternalModelClass();
	this->m_Shader->Render(direct->GetDeviceContext(), modelClass->GetIndexCount(), model, view, projection, modelClass->GetTexture());
}
