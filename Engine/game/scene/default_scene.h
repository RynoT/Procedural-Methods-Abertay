#pragma once

#include "scene.h"

#include "../entity/model_entity.h"

struct GridCell;

class Player;
class WorldGrid;
class PostProcessor;
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

	virtual void RenderScene(D3DClass* direct, const D3DXMATRIX& projection) override;

protected:
	bool UpdateMap(const float& delta);

	bool UpdateSurface(const float& delta);

private:
	Player *m_Player;
	GameState m_State;

	bool m_bTransitioning, m_bSurfaceTransition;
	WorldGrid *m_WorldGrid;
	ModelEntity *m_IslandHover;
	GridCell const*m_HoveredCell;

	D3DXMATRIX m_LastProjection, m_LastView;
};