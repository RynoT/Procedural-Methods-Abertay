#pragma once

#include "scene.h"

class Leaf;
class ColorShaderClass;

// DungeonScene displays a procedurally generated dungeon.
// The dungeon uses BSP to generate
class DungeonScene : public Scene
{
public:
	DungeonScene(D3DClass *d3d, const HWND& hwnd, InputClass *input);
	virtual ~DungeonScene();
	
	// Generate the randomized dungeon
	void GenerateDungeon(D3DClass* d3d);

	// When the game tries to go into the scene it's already on, generate a new dungeon
	virtual void OnRefresh(D3DClass* d3d) override;

	virtual bool Update(const float& delta) override;

	virtual void RenderScene(D3DClass* direct, const D3DXMATRIX& projection) override;

private:
	Leaf *m_RootLeaf;
	ColorShaderClass *m_ColorShader;
};
