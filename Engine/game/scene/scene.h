#pragma once

#include "../../d3dclass.h"

class Camera;
class InputClass;
class TextureShaderClass;

// List of all possible scene types
enum SceneId
{
	UNDEFINED, DEFAULT, DUNGEON
};

class Scene
{
protected:
	Scene(const SceneId& id, D3DClass *d3d, const HWND& hwnd, InputClass *input);

public:
	virtual ~Scene();

	// Get mouse movement dx since last update
	float GetCameraDX() const;

	// Get mouse movement dy since last update
	float GetCameraDY() const;

	virtual void OnResize(D3DClass *d3d, const int& width, const int& height);

	// When the game tries to go into the scene it's already on
	virtual void OnRefresh(D3DClass* d3d) { }

	virtual bool Update(const float& delta);

	void Render(D3DClass* direct, const D3DXMATRIX& projection);

	inline const SceneId& GetSceneId() const { return this->m_Id; }

protected:
	virtual void RenderScene(D3DClass* direct, const D3DXMATRIX& projection) = 0;

	void SetCamera(Camera *camera);

	inline Camera* GetCamera() const { return this->m_Camera; }

	inline TextureShaderClass* GetTextureShader() const { return this->m_TextureShader; }

	D3DClass *m_Direct3D;
	InputClass *m_Input;

private:
	SceneId m_Id;
	Camera *m_Camera;
	TextureShaderClass *m_TextureShader;
	
	// Post processing effects. We store the pointers to all effects so that we can enable and disable them.
	class PostProcessor *m_PostProcessor;
	class PostEffect *m_BlurEffect, *m_VignatteEffect, *m_InvertEffect, *m_ConvBlurEffect, *m_EdgeEffect;
};