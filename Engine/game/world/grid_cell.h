#pragma once

#include "../../d3dclass.h"

struct Vector3f;
class Island;
class D3DClass;
class ModelEntity;
class TextureShaderClass;

struct GridCell
{
	GridCell();

	void Destroy();

	void GenerateIsland(D3DClass *d3d, const float& worldSize);

	bool IsHovered(const float& x, const float& y, const D3DXMATRIX& projection, const D3DXMATRIX& view) const;

	void Render(D3DClass* direct, const D3DXMATRIX& projection, const D3DXMATRIX& view, const D3DXMATRIX& model) const;

	inline const bool& IsReady() const { return this->m_bReady; }

	inline void SetShader(TextureShaderClass *shader) { this->m_Shader = shader; }

	Island *m_Island;
	int m_CellX, m_CellY;

private:
	static int Hash(const int& x, const int& y);

	bool m_bReady;
	TextureShaderClass *m_Shader;
};