#pragma once

#include "scene.h"

class Player;
class WorldGrid;
class TextureShaderClass;

class DefaultScene : public Scene
{
public:
	DefaultScene(D3DClass *d3d, const HWND& hwnd, InputClass *input);
	virtual ~DefaultScene();

	virtual void Update(const float& delta) override;

	virtual void Render(D3DClass* direct, D3DXMATRIX& projection) override;

private:
	Player *m_Player;
	WorldGrid *m_WorldGrid;

	TextureShaderClass *m_TextureShader;
};