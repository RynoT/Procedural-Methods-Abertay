#pragma once

#include "scene.h"
#include "../entity/model_entity.h"
#include "../../modelclass.h"

struct GridCell;
class Player;
class WorldGrid;
class IslandHoverModel;
class TextureShaderClass;
class VerticalBlurShaderClass;
class HorizontalBlurShaderClass;
class RenderTextureClass;
class OrthoWindowClass;

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

protected:
	bool UpdateMap(const float& delta);

	bool UpdateSurface(const float& delta);

	void RenderUpSample(D3DClass* direct, const D3DXMATRIX& world, const D3DXMATRIX& view) const;

	void RenderVerticalSample(D3DClass* direct, const D3DXMATRIX& world, const D3DXMATRIX& view) const;

	void RenderHorizontalSample(D3DClass* direct, const D3DXMATRIX& world, const D3DXMATRIX& view) const;

	void RenderDownSample(D3DClass* direct, const D3DXMATRIX& world, const D3DXMATRIX& view) const;

	void Render2DTextureScene(D3DClass* direct, const D3DXMATRIX& world, const D3DXMATRIX& view) const;

private:
	GameState m_State;

	Player *m_Player;
	ModelEntity *m_IslandHover;

	bool m_bTransitioning, m_bSurfaceTransition;
	GridCell const*m_HoveredCell;
	WorldGrid *m_WorldGrid;
	ModelClass* m_Model;

	TextureShaderClass *m_TextureShader;

	D3DXMATRIX m_LastProjection, m_LastView;

	bool m_bBlurMode;
	VerticalBlurShaderClass* m_VerticalBlurShader;
	HorizontalBlurShaderClass* m_HorizontalBlurShader;
	RenderTextureClass *m_RenderTexture, *m_DownSampleTexure, *m_HorizontalBlurTexture, *m_VerticalBlurTexture, *m_UpSampleTexure;

	OrthoWindowClass *m_Window, *m_SmallWindow;
};