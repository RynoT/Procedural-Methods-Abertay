#pragma once

#include <vector>

#include "../../../d3dclass.h"

class PostEffect;
class OrthoWindowClass;
class RenderTextureClass;
class TextureShaderClass;

#define DOWN_SAMPLE_WIDTH (width / 2)
#define DOWN_SAMPLE_HEIGHT (height / 2)

class PostProcessor
{
public:
	PostProcessor(D3DClass* direct, const HWND& hwnd, TextureShaderClass *shader);
	~PostProcessor();

	void OnResize(D3DClass *d3d, const int& width, const int& height);

	void Update(const float& delta);

	void Render(D3DClass *direct);

	inline void AddEffect(PostEffect *effect) { this->m_Effects.push_back(effect); }

	inline OrthoWindowClass* GetWindow() const { return this->m_Window; }

	inline OrthoWindowClass* GetSmallWindow() const { return this->m_SmallWindow; }

	inline TextureShaderClass* GetTextureShader() const { return this->m_TextureShader; }

	inline RenderTextureClass* GetSceneRenderTexture() const { return this->m_RenderTexture; }

private:
	int m_LastWidth, m_LastHeight;
	std::vector<PostEffect*> m_Effects;

	D3DXMATRIX m_WindowViewMatrix;
	RenderTextureClass *m_RenderTexture;
	TextureShaderClass *m_TextureShader;
	OrthoWindowClass *m_Window, *m_SmallWindow;
};