#pragma once

#include "scene.h"
#include "../entity/model_entity.h"

struct GridCell;
class Player;
class WorldGrid;
class IslandHoverModel;
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

	virtual bool Update(const float& delta) override;

	virtual void Render(D3DClass* direct, const D3DXMATRIX& projection) override;

private:
	GameState m_State;

	Player *m_Player;
	ModelEntity *m_IslandHover;

	bool m_bTransitioning, m_bSurfaceTransition;
	GridCell const*m_HoveredCell;
	WorldGrid *m_WorldGrid;

	TextureShaderClass *m_TextureShader;

	D3DXMATRIX m_LastProjection, m_LastView;
};