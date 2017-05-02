#include "world_grid.h"

#include <assert.h>

#include "grid_cell.h"
#include "../entity/island.h"

WorldGrid::WorldGrid(const float& worldSize, const int& size)
	: m_Shader(nullptr), m_WorldSize(worldSize), m_GridSize(size), m_OffsetX(0), m_OffsetY(0)
{
	this->m_Cells = new GridCell[size * size];
	for (int i = 0; i < size * size; i++)
	{
		this->m_Cells[i].m_CellX = i % size - size / 2;
		this->m_Cells[i].m_CellY = i / size - size / 2;
	}

	// Size must be odd so that we can have a center
	assert(size % 2 == 1 && "Size must be an odd number");
}

WorldGrid::~WorldGrid()
{
	if (this->m_Cells != nullptr)
	{
		delete[] this->m_Cells;
		this->m_Cells = nullptr;
	}
}

void WorldGrid::SetShader(TextureShaderClass* shader)
{
	this->m_Shader = shader;
	for (int i = 0; i < this->GetCellCount(); i++)
	{
		this->m_Cells[i].SetShader(shader);
	}
}

void WorldGrid::Update(D3DClass *d3d, const float& worldX, const float& worldY) const
{
	int x = this->GetGridCoord(worldX), y = this->GetGridCoord(worldY);
	const int halfSize = this->m_GridSize / 2;
	int offsetX = x - halfSize, offsetY = y - halfSize;
	if (this->m_OffsetX != offsetX || this->m_OffsetY != offsetY)
	{
		//int dx = offsetX - this->m_OffsetX, dy = offsetY - this->m_OffsetY;
		for (int i = 0; i < this->m_GridSize * this->m_GridSize; i++)
		{
			GridCell& cell = this->m_Cells[i];
			if (cell.m_CellX < offsetX - halfSize) //if left
			{
				cell.Destroy();
				cell.m_CellX = offsetX + halfSize;
			}
			if (cell.m_CellY < offsetY - halfSize) //if below
			{
				cell.Destroy();
				cell.m_CellY = offsetY + halfSize;
			}
			if (cell.m_CellX > offsetX + halfSize) //if right
			{
				cell.Destroy();
				cell.m_CellX = offsetX - halfSize;
			}
			if (cell.m_CellY > offsetY + halfSize) //if above
			{
				cell.Destroy();
				cell.m_CellY = offsetY - halfSize;
			}
		}
	}
	for (int i = 0; i < this->GetCellCount(); i++)
	{
		GridCell& cell = this->m_Cells[i];
		if (!cell.IsReady())
		{
			cell.GenerateIsland(d3d, this->m_WorldSize);
		}
	}
}

void WorldGrid::Render(D3DClass* direct, const D3DXMATRIX& projection, const D3DXMATRIX& view) const
{
	if (this->m_Shader == nullptr)
	{
		return;
	}
	for (int i = 0; i < this->GetCellCount(); i++)
	{
		GridCell& cell = this->m_Cells[i];
		if (cell.m_Island == nullptr)
		{
			continue;
		}
		cell.m_Island->Render(direct, projection, view);
	}
}
