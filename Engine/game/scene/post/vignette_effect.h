#pragma once

#include "post_effect.h"

class VignetteShaderClass;

// The VignetteEffect is a simple well-known post processing effect that adds shade to the borders of the window.
class VignetteEffect : public PostEffect
{
public:
	VignetteEffect(D3DClass* d3d, const HWND& hwnd);
	~VignetteEffect();

	virtual void OnResize(D3DClass* d3d, const int& width, const int& height) override;

protected:
	virtual void RenderEffect(PostProcessor* processor, D3DClass* direct, const D3DXMATRIX& world, const D3DXMATRIX& view) const override;

private:
	RenderTextureClass *m_VignetteTexture;
	VignetteShaderClass *m_VignetteShader;
};
