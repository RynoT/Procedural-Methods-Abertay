#include "grid_cell.h"

#include <cstdlib>

#include "../model/model.h"
#include "../../d3dclass.h"
#include "../../timerclass.h"
#include "../entity/island.h"
#include "../../textureshaderclass.h"
#include "../model/collision/aabb.h"
#include "../../applicationclass.h"

#define ISLAND_SPAWN_CHANCE 0.35f //spawns every x percent
#define ISLAND_BORDER_OFFSET 20.0f
#define MAX_ISLAND_RADIUS 60.0f
#define MIN_ISLAND_RADIUS 450.0f
#define ISLAND_Y_OFFSET 800.0f
#define COLLISION_BOX_SCALE 1.25f

#define CENTER_ALL_ISLANDS false

#define SEED TimerClass::SeedOffset // 1000 // Use time as seed 

#define pseudo_seed(x, y) srand(GridCell::Hash(x, y))
#define pseudo_random() (rand() / float(RAND_MAX)) // Random number between 0-1

GridCell::GridCell() : m_Island(nullptr), m_CellX(0), m_CellY(0), m_bReady(false), m_Shader(nullptr)
{
}

void GridCell::Destroy()
{
	this->m_bReady = false;
	if (this->m_Island != nullptr)
	{
		delete this->m_Island;
		this->m_Island = nullptr;
	}
}

// http://stackoverflow.com/questions/37128451/random-number-generator-with-x-y-coordinates-as-seed
int GridCell::Hash(const int& x, const int& y)
{
	int h = x * 374761393 + y * 668265263 + SEED;
	h = (h ^ (h >> 13)) * 1274126177;
	return h ^ (h >> 16);
}

void GridCell::GenerateIsland(D3DClass *d3d, const float& size)
{
	if (this->m_Island != nullptr)
	{
		delete this->m_Island;
		this->m_Island = nullptr;
	}

	// We must seed the random using x and y so that our random numbers are always consistent
	pseudo_seed(this->m_CellX, this->m_CellY);

	this->m_bReady = true;
	if (pseudo_random() > ISLAND_SPAWN_CHANCE)
	{
		return;
	}

	// Generate random radius for island
	float radius = MIN_ISLAND_RADIUS + (MAX_ISLAND_RADIUS - MIN_ISLAND_RADIUS) * pseudo_random();

	// Generate relative position within grid according to size and radius
	float rx = radius + (size - radius * 2.0f - ISLAND_BORDER_OFFSET) * pseudo_random(),
		ry = radius + (size - radius * 2.0f - ISLAND_BORDER_OFFSET) * pseudo_random();
	float y = -ISLAND_Y_OFFSET / 2.0f + ISLAND_Y_OFFSET * pseudo_random();
	if (CENTER_ALL_ISLANDS)
	{
		rx = ry = y = 0.0f;
	}

	this->m_Island = new Island(d3d->GetDevice(), Island::GetType(pseudo_random()));
	this->m_Island->SetScale(radius);
	this->m_Island->SetPosition(this->m_CellX * size + rx, y, this->m_CellY * size + ry);
	this->m_Island->SetRenderMethod([this](D3DClass* direct, const D3DXMATRIX& projection,
		const D3DXMATRIX& view, const D3DXMATRIX& model)->void { this->Render(direct, projection, view, model); });
}

// Use matrix maths to get the screen bounds of the island model. Then check to see if (x, y) are within those bounds.
bool GridCell::IsHovered(const float& x, const float& y, const D3DXMATRIX& projection, const D3DXMATRIX& view) const
{
	if (this->m_Island == nullptr)
	{
		return false;
	}
	D3DXMATRIX model, transform;
	D3DXMatrixIdentity(&model);

	D3DXMatrixScaling(&transform, VECTOR3_SPLIT(this->m_Island->GetScale()));
	D3DXMatrixMultiply(&model, &model, &transform);

	D3DXMatrixTranslation(&transform, VECTOR3_SPLIT(this->m_Island->GetPosition()));
	D3DXMatrixMultiply(&model, &model, &transform);

	D3DXMATRIX pvm = model * view * projection;

	CollisionAABB *collision = (CollisionAABB*)this->m_Island->GetInternalModel()->GetCollision();
	D3DXVECTOR3 min = collision->GetMin() * COLLISION_BOX_SCALE, max = collision->GetMax() * COLLISION_BOX_SCALE;

	D3DXVECTOR3 outA, outB;
	D3DXVec3TransformCoord(&outA, &min, &pvm);
	D3DXVec3TransformCoord(&outB, &max, &pvm);

	outA.x = (outA.x + 1.0f) * ApplicationClass::SCREEN_WIDTH * 0.5f;
	outA.y = (1.0f - outA.y) * ApplicationClass::SCREEN_HEIGHT * 0.5f;

	outB.x = (outB.x + 1.0f) * ApplicationClass::SCREEN_WIDTH * 0.5f;
	outB.y = (1.0f - outB.y) * ApplicationClass::SCREEN_HEIGHT * 0.5f;

	return x >= min(outA.x, outB.x) && x <= max(outA.x, outB.x)
		&& y >= min(outA.y, outB.y) && y <= max(outA.y, outB.y);
}

void GridCell::Render(D3DClass* direct, const D3DXMATRIX& projection, const D3DXMATRIX& view, const D3DXMATRIX& model) const
{
	if (this->m_Shader == nullptr || this->m_Island == nullptr)
	{
		return;
	}
	Model *imodel = this->m_Island->GetInternalModel();
	this->m_Shader->Render(direct->GetDeviceContext(), imodel->GetIndexCount(), model, view, projection, imodel->GetTexture());
}
