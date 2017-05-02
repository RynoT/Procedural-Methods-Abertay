#pragma once

#include "convolution_effect.h"

class EdgeDetectionShaderClass;

// EdgeDetectEffect shows edges in the scene. This works by blurring the scene and then subtracting the blur from the scene.
// This technique for edge detection is well known and reasonably effective.
class EdgeDetectEffect : public ConvolutionEffect
{
public:
	EdgeDetectEffect(D3DClass* d3d, const HWND& hwnd);
	~EdgeDetectEffect();

	virtual void OnResize(D3DClass* d3d, const int& width, const int& height) override;

protected:
	virtual void RenderEffect(PostProcessor* processor, D3DClass* direct, const D3DXMATRIX& world, const D3DXMATRIX& view) const override;

private:
	RenderTextureClass *m_EdgeTexture;
	EdgeDetectionShaderClass *m_EdgeDetectShader;
};
