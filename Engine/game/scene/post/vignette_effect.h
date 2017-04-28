#pragma once

#include "post_effect.h"

class VignetteShaderClass;

class VignetteEffect : public PostEffect
{
public:
	VignetteEffect(D3DClass* d3d, const HWND& hwnd);
	~VignetteEffect();

	virtual void OnResize(D3DClass* d3d, const int& width, const int& height) override;

protected:
	virtual void RenderEffect(PostProcessor* processor, D3DClass* direct, const D3DXMATRIX& world, const D3DXMATRIX& view) const override;

private:
	RenderTextureClass *m_VignetteTexure;
	VignetteShaderClass *m_VignetteShader;
};
