#include "world_grid.h"

#include "grid_cell.h"
#include "../entity/model_entity.h"

WorldGrid::WorldGrid(const float& worldSize, const int& size)
	: m_Shader(nullptr), m_WorldSize(worldSize), m_GridSize(size), m_bDirty(true), m_OffsetX(0), m_OffsetY(0)
{
	this->m_Cells = new GridCell[size * size];
}

WorldGrid::~WorldGrid()
{
	if (this->m_Cells != nullptr)
	{
		delete[] this->m_Cells;
		this->m_Cells = nullptr;
	}
}

void WorldGrid::Update(D3DClass *d3d, const float& worldX, const float& worldY)
{
	int x = this->GetGridCoord(worldX), y = this->GetGridCoord(worldY);
	if(this->m_bDirty)
	{
		for (int i = 0; i < this->m_GridSize * this->m_GridSize; i++)
		{
			GridCell& cell = this->m_Cells[i];
			int cx = i % this->m_GridSize, cy = i / this->m_GridSize;
			if (cell.m_CellX != cx || cell.m_CellY != cy)
			{
				cell.m_CellX = cx;
				cell.m_CellY = cy;

				this->m_Cells[i].GenerateIsland(d3d, cx, cy, this->m_WorldSize);
			}
		}
		this->m_bDirty = false;
	}
}

void WorldGrid::Render(D3DClass* direct, D3DXMATRIX& projection, D3DXMATRIX& view) const
{
	if(this->m_Shader == nullptr)
	{
		return;
	}
	for (int i = 0; i < this->m_GridSize * this->m_GridSize; i++)
	{
		GridCell& cell = this->m_Cells[i];
		if(cell.m_Model == nullptr)
		{
			continue;
		}
		cell.m_Model->Render(direct, projection, view);
	}
}
