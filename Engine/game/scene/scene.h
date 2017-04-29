#pragma once

#include "../../d3dclass.h"

class Camera;
class InputClass;
class TextureShaderClass;

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

	float GetCameraDX() const;

	float GetCameraDY() const;

	virtual void OnResize(D3DClass *d3d, const int& width, const int& height);

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
	
	class PostProcessor *m_PostProcessor;
	class PostEffect *m_BlurEffect, *m_VignatteEffect, *m_InvertEffect, *m_ConvBlurEffect, *m_EdgeEffect;
};