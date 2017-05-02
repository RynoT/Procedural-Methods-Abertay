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

	// Destroy the island in this cell
	void Destroy();

	// Generate an island in this cell
	void GenerateIsland(D3DClass *d3d, const float& worldSize);

	// Whether or not the screen x, y are hovered over this island model
	bool IsHovered(const float& x, const float& y, const D3DXMATRIX& projection, const D3DXMATRIX& view) const;

	// Render the island model 
	void Render(D3DClass* direct, const D3DXMATRIX& projection, const D3DXMATRIX& view, const D3DXMATRIX& model) const;

	// Whether or not this cell has had its island generated. (Even if the pseduorandom says we should have no island)
	inline const bool& IsReady() const { return this->m_bReady; }

	// Set shader required for rendering the islands
	inline void SetShader(TextureShaderClass *shader) { this->m_Shader = shader; }

	Island *m_Island;
	int m_CellX, m_CellY;

private:
	// Hash the x and y coordinates
	static int Hash(const int& x, const int& y);

	bool m_bReady;
	TextureShaderClass *m_Shader;
};