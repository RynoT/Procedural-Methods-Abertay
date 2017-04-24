#pragma once

#include "scene.h"

class ColorShaderClass;

class DungeonScene : public Scene
{
public:
	DungeonScene(D3DClass *d3d, const HWND& hwnd, InputClass *input);
	virtual ~DungeonScene();

	virtual bool Update(const float& delta) override;

	virtual void Render(D3DClass* direct, const D3DXMATRIX& projection) override;

private:
	ColorShaderClass *m_ColorShader;
};
