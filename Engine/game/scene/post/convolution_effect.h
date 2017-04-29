#pragma once

#include "post_effect.h"

class ConvolutionShaderClass;

class ConvolutionEffect : public PostEffect
{
public:
	ConvolutionEffect(D3DClass* d3d, const HWND& hwnd);
	virtual ~ConvolutionEffect();

	virtual void OnResize(D3DClass* d3d, const int& width, const int& height) override;

	inline void SetIterationCount(const int& count) { this->m_Iterations = count; }

	inline void SetConvolutionMatrix(const D3DXMATRIX& matrix) { this->m_ConvolutionMatrix = matrix; }

	inline void SetConvolutionMatrix(const float& m00, const float& m01, const float& m02,
		const float& m10, const float& m11, const float& m12,
		const float& m20, const float& m21, const float& m22)
	{
		this->m_ConvolutionMatrix = D3DXMATRIX(m00, m01, m02, 0.0f, m10, m11, m12, 0.0f, m20, m21, m22, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	}

protected:
	virtual void RenderEffect(PostProcessor* processor, D3DClass* direct, const D3DXMATRIX& world, const D3DXMATRIX& view) const override;

	void RenderConvolution(PostProcessor* processor, D3DClass* direct, const D3DXMATRIX& world, const D3DXMATRIX& view) const;

	RenderTextureClass *m_ConvolutionTexture;

private:
	int m_Iterations;
	D3DXMATRIX m_ConvolutionMatrix;

	ConvolutionShaderClass *m_ConvolutionShader;
};
