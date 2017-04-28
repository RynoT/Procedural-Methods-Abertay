#pragma once

#include "post_effect.h"

class InvertShaderClass;

class InvertEffect : public PostEffect
{
public:
	InvertEffect(D3DClass* d3d, const HWND& hwnd);
	~InvertEffect();

	virtual void OnResize(D3DClass* d3d, const int& width, const int& height) override;

protected:
	virtual void RenderEffect(PostProcessor* processor, D3DClass* direct, const D3DXMATRIX& world, const D3DXMATRIX& view) const override;

private:
	RenderTextureClass *m_InvertTexure;
	InvertShaderClass *m_InvertShader;
};
