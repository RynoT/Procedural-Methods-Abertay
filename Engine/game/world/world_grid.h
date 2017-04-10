#pragma once

#define WORLD_GRID_DEFAULT_SIZE 5

#include "grid_cell.h"
#include "../../d3dclass.h"

class D3DClass;
class TextureShaderClass;

class WorldGrid
{
public:
	WorldGrid(const float& worldSize, const int& size = WORLD_GRID_DEFAULT_SIZE);
	~WorldGrid();

	void SetShader(TextureShaderClass *shader);

	void Update(D3DClass *d3d, const float& worldX, const float& worldY);

	void Render(D3DClass* direct, const D3DXMATRIX& projection, const D3DXMATRIX& view) const;

	inline const int& GetGridSize() const { return this->m_GridSize; }

protected:
	inline float GetWorldOffset() const { return (this->m_WorldSize * this->m_GridSize) / 2.0f; }

	inline int GetGridCoord(const float& world) const { return int((world + this->GetWorldOffset()) / this->m_WorldSize); }

	inline int GetCellIndex(const int& x, const int& y) const 
	{ 
		return (x + this->m_OffsetX) % this->m_GridSize + (y + this->m_OffsetY) % this->m_GridSize * this->m_GridSize;
	}

	inline GridCell& GetCell(const int& x, const int& y) const { return this->m_Cells[this->GetCellIndex(x, y)]; }

private:
	GridCell *m_Cells;

	TextureShaderClass *m_Shader;

	const float m_WorldSize;

	const int m_GridSize;

	bool m_bDirty;

	int m_OffsetX, m_OffsetY;
};