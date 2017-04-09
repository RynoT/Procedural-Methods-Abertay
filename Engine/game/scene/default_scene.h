#pragma once

#include "scene.h"

class Player;
class WorldGrid;
class TextureShaderClass;

enum GameState
{
	Undefined,
	Map, Surface
};

class DefaultScene : public Scene
{
public:
	DefaultScene(D3DClass *d3d, const HWND& hwnd, InputClass *input);
	virtual ~DefaultScene();

	void SetState(const GameState& state);

	virtual void Update(const float& delta) override;

	virtual void Render(D3DClass* direct, D3DXMATRIX& projection) override;

private:
	GameState m_State;

	Player *m_Player;
	WorldGrid *m_WorldGrid;

	TextureShaderClass *m_TextureShader;
};