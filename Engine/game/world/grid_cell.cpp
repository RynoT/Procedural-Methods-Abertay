#include "grid_cell.h"

#include <cstdlib>

#include "../model/model.h"
#include "../../d3dclass.h"
#include "../../timerclass.h"
#include "../../modelclass.h"
#include "../entity/model_entity.h"
#include "../../textureshaderclass.h"

//#define MIN_ISLAND_RADIUS_PERC 0.08f
//#define MAX_ISLAND_RADIUS_PERC 0.32f

//#define MIN_ISLAND_RADIUS (size * MIN_ISLAND_RADIUS_PERC)
//#define MAX_ISLAND_RADIUS (size * MAX_ISLAND_RADIUS_PERC)

#define ISLAND_SPAWN_CHANCE 1.0f//0.35f //spawns every x percent
#define ISLAND_BORDER_OFFSET 100.0f
#define MAX_ISLAND_RADIUS 1000.0f
#define MIN_ISLAND_RADIUS 5000.0f

#define CENTER_ALL_ISLANDS true//false

#define SEED TimerClass::SeedOffset // Use time as seed

#define pseudo_seed(x, y) srand(GridCell::Hash(x, y))
#define pseudo_random() (rand() / float(RAND_MAX)) // Random number between 0-1

GridCell::GridCell() : m_CellX(0), m_CellY(0), m_Model(nullptr), m_bReady(false), m_Shader(nullptr)
{
}

void GridCell::Destroy()
{
	this->m_bReady = false;
	if(this->m_Model != nullptr)
	{
		delete this->m_Model;
		this->m_Model = nullptr;
	}
}

int GridCell::Hash(const int& x, const int& y)
{
	int h = x * 374761393 + y * 668265263 + SEED;
	h = (h ^ (h >> 13)) * 1274126177;
	return h ^ (h >> 16);
}

void GridCell::GenerateIsland(D3DClass *d3d, const float& size)
{
	if (this->m_Model != nullptr)
	{
		delete this->m_Model;
		this->m_Model = nullptr;
	}

	// We must seed the random using x and y so that our random numbers are always consistent
	pseudo_seed(this->m_CellX, this->m_CellY);

	this->m_bReady = true;
	if(pseudo_random() > ISLAND_SPAWN_CHANCE)
	{
		return;
	}

	// Generate random radius for island
	float radius = MIN_ISLAND_RADIUS + (MAX_ISLAND_RADIUS - MIN_ISLAND_RADIUS) * pseudo_random();

	// Generate relative position within grid according to size and radius
	float rx = radius + (size - radius * 2.0f - ISLAND_BORDER_OFFSET) * pseudo_random(),
		ry = radius + (size - radius * 2.0f - ISLAND_BORDER_OFFSET) * pseudo_random();
	if(CENTER_ALL_ISLANDS)
	{
		rx = ry = 0.0f;
	}

	this->m_Model = new ModelEntity;
	this->m_Model->SetScale(radius);
	this->m_Model->SetPosition(this->m_CellX * size + rx, this->m_CellY * size + ry, 0.0f);
	this->m_Model->SetFrom(d3d->GetDevice(), "data/models/cube.txt", L"data/models/seafloor.dds");
	this->m_Model->SetRenderMethod([this](D3DClass* direct, const D3DXMATRIX& projection,
		const D3DXMATRIX& view, const D3DXMATRIX& model)->void { this->Render(direct, projection, view, model); });
}

void GridCell::Render(D3DClass* direct, const D3DXMATRIX& projection, const D3DXMATRIX& view, const D3DXMATRIX& model) const
{
	if (this->m_Shader == nullptr)
	{
		return;
	}
	Model *imodel = this->m_Model->GetInternalModel();
	this->m_Shader->Render(direct->GetDeviceContext(), imodel->GetIndexCount(), model, view, projection, imodel->GetTexture());
}
