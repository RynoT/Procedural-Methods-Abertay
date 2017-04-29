#include "edge_detect_effect.h"

#include "post_processor.h"
#include "../../../applicationclass.h"
#include "../../../orthowindowclass.h"
#include "../../../rendertextureclass.h"
#include "../../../textureshaderclass.h"
#include "../../../edgedetectionshaderclass.h"

EdgeDetectEffect::EdgeDetectEffect(D3DClass* d3d, const HWND& hwnd) 
: ConvolutionEffect(d3d, hwnd), m_EdgeTexture(nullptr)
{
	this->m_EdgeDetectShader = new EdgeDetectionShaderClass;
	this->m_EdgeDetectShader->Initialize(d3d->GetDevice(), hwnd);

	ConvolutionEffect::SetIterationCount(1);
	ConvolutionEffect::SetConvolutionMatrix(1, 2, 1, 2, 4, 2, 1, 2, 1);
}

EdgeDetectEffect::~EdgeDetectEffect()
{
	if (this->m_EdgeTexture != nullptr)
	{
		this->m_EdgeTexture->Shutdown();
		delete this->m_EdgeTexture;
		this->m_EdgeTexture = nullptr;
	}
	if (this->m_EdgeDetectShader != nullptr)
	{
		this->m_EdgeDetectShader->Shutdown();
		delete this->m_EdgeDetectShader;
		this->m_EdgeDetectShader = nullptr;
	}
}

void EdgeDetectEffect::OnResize(D3DClass* d3d, const int& width, const int& height)
{
	if (this->m_EdgeTexture != nullptr)
	{
		this->m_EdgeTexture->Shutdown();
		delete this->m_EdgeTexture;
	}
	this->m_EdgeTexture = new RenderTextureClass;
	this->m_EdgeTexture->Initialize(d3d->GetDevice(), width, height, SCREEN_DEPTH, SCREEN_NEAR);

	ConvolutionEffect::OnResize(d3d, width, height);
}

void EdgeDetectEffect::RenderEffect(PostProcessor* processor, D3DClass* direct, const D3DXMATRIX& world, const D3DXMATRIX& view) const
{
	ConvolutionEffect::RenderConvolution(processor, direct, world, view);

	this->m_EdgeTexture->SetRenderTarget(direct->GetDeviceContext());
	this->m_EdgeTexture->ClearRenderTarget(direct->GetDeviceContext(), 0.0f, 1.0f, 0.0f, 1.0f);

	D3DXMATRIX orthoMatrix;
	this->m_EdgeTexture->GetOrthoMatrix(orthoMatrix);

	processor->GetWindow()->Render(direct->GetDeviceContext());
	this->m_EdgeDetectShader->Render(direct->GetDeviceContext(), processor->GetWindow()->GetIndexCount(), world, view, orthoMatrix, 
		processor->GetSceneRenderTexture()->GetShaderResourceView(), ConvolutionEffect::m_ConvolutionTexture->GetShaderResourceView());

	PostEffect::CompleteRenderEffect(processor, direct, this->m_EdgeTexture, world, view);
}
