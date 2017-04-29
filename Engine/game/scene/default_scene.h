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

	virtual void OnResize(D3DClass *d3d, const int& width, const int& height) override;

	virtual bool Update(const float& delta) override;

	virtual void Render(D3DClass* direct, const D3DXMATRIX& projection) override;

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

	PostProcessor *m_PostProcessor;
	TextureShaderClass *m_TextureShader;
	D3DXMATRIX m_LastProjection, m_LastView;

	class PostEffect *m_BlurEffect, *m_VignatteEffect, *m_InvertEffect, *m_ConvolutionEffect;
};