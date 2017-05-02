#pragma once

#include "post_effect.h"

class VerticalBlurShaderClass;
class HorizontalBlurShaderClass;

// Blur effect using vertical and horizontal blur shaders.
// Blur is applied to downsampled texture and then upsampled for performance
class BlurEffect : public PostEffect
{
public:
	BlurEffect(D3DClass* d3d, const HWND& hwnd);
	virtual ~BlurEffect();

	virtual void OnResize(D3DClass* d3d, const int& width, const int& height) override;

protected:
	virtual void RenderEffect(PostProcessor* processor, D3DClass* direct, const D3DXMATRIX& world, const D3DXMATRIX& view) const override;

	void RenderUpSample(PostProcessor* processor, D3DClass* direct, const D3DXMATRIX& world, const D3DXMATRIX& view) const;

	void RenderVerticalSample(PostProcessor* processor, D3DClass* direct, const D3DXMATRIX& world, const D3DXMATRIX& view) const;

	void RenderHorizontalSample(PostProcessor* processor, D3DClass* direct, const D3DXMATRIX& world, const D3DXMATRIX& view) const;

	void RenderDownSample(PostProcessor* processor, D3DClass* direct, const D3DXMATRIX& world, const D3DXMATRIX& view) const;

private:
	VerticalBlurShaderClass *m_VerticalBlurShader;
	HorizontalBlurShaderClass *m_HorizontalBlurShader;

	RenderTextureClass *m_DownSampleTexure, *m_HorizontalBlurTexture, *m_VerticalBlurTexture, *m_UpSampleTexure;
};
