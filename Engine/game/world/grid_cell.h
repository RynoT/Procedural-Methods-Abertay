#pragma once

class D3DClass;
class ModelEntity;

struct GridCell
{
	GridCell();

	void GenerateIsland(D3DClass *d3d, const int& cellX, const int& cellY, const float& worldSize);

	int m_CellX, m_CellY;
	ModelEntity *m_Model;
};