#pragma once

#include <vector>

#include "../../../d3dclass.h"

class PostEffect;
class OrthoWindowClass;
class RenderTextureClass;
class TextureShaderClass;

#define DOWN_SAMPLE_WIDTH (width / 2)
#define DOWN_SAMPLE_HEIGHT (height / 2)

// PostProcessor stores PostEffects and will call them all in sequence during a frame render.
class PostProcessor
{
public:
	PostProcessor(D3DClass* direct, const HWND& hwnd, TextureShaderClass *shader);
	~PostProcessor();

	void OnResize(D3DClass *d3d, const int& width, const int& height);

	void Update(const float& delta);

	void Render(D3DClass *direct);

	// Add an effect to the render queue. This is order sensitive.
	inline void AddEffect(PostEffect *effect) { this->m_Effects.push_back(effect); }

	inline OrthoWindowClass* GetWindow() const { return this->m_Window; }

	inline OrthoWindowClass* GetSmallWindow() const { return this->m_SmallWindow; }

	inline TextureShaderClass* GetTextureShader() const { return this->m_TextureShader; }

	inline RenderTextureClass* GetSceneRenderTexture() const { return this->m_RenderTexture; }

private:
	int m_LastWidth, m_LastHeight;
	std::vector<PostEffect*> m_Effects;

	D3DXMATRIX m_WindowViewMatrix; // View matrix for rendering 2d textures to the whole screen
	RenderTextureClass *m_RenderTexture; // The output texture used by all PostEffects
	TextureShaderClass *m_TextureShader; // Standard texture shader
	OrthoWindowClass *m_Window, *m_SmallWindow; // Window objects that can be used for rendering to the dimensions of the screen
};