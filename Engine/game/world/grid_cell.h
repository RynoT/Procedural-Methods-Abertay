#pragma once

#include "../../d3dclass.h"

class D3DClass;
class ModelEntity;
class TextureShaderClass;

struct GridCell
{
	GridCell();

	void GenerateIsland(D3DClass *d3d, const int& cellX, const int& cellY, const float& worldSize);

	void Render(D3DClass* direct, const D3DXMATRIX& projection, const D3DXMATRIX& view, const D3DXMATRIX& model) const;

	inline void SetShader(TextureShaderClass *shader) { this->m_Shader = shader; }

	int m_CellX, m_CellY;
	ModelEntity *m_Model;

private:
	TextureShaderClass *m_Shader;
};