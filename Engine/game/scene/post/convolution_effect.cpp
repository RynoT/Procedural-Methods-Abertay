#include "convolution_effect.h"

#include "post_processor.h"
#include "../../../applicationclass.h"
#include "../../../orthowindowclass.h"
#include "../../../rendertextureclass.h"
#include "../../../textureshaderclass.h"
#include "../../../convolutionshaderclass.h"

ConvolutionEffect::ConvolutionEffect(D3DClass* d3d, const HWND& hwnd)
: PostEffect(d3d, hwnd), m_Iterations(1), m_ConvolutionTexture(nullptr), m_ConvolutionShader(nullptr)
{
	D3DXMatrixIdentity(&this->m_ConvolutionMatrix);

	this->m_ConvolutionShader = new ConvolutionShaderClass;
	this->m_ConvolutionShader->Initialize(d3d->GetDevice(), hwnd);
}

ConvolutionEffect::~ConvolutionEffect()
{
	if (this->m_ConvolutionTexture != nullptr)
	{
		this->m_ConvolutionTexture->Shutdown();
		delete this->m_ConvolutionTexture;
		this->m_ConvolutionTexture = nullptr;
	}
	if (this->m_ConvolutionShader != nullptr)
	{
		this->m_ConvolutionShader->Shutdown();
		delete this->m_ConvolutionShader;
		this->m_ConvolutionShader = nullptr;
	}
}

void ConvolutionEffect::OnResize(D3DClass* d3d, const int& width, const int& height)
{
	if (this->m_ConvolutionTexture != nullptr)
	{
		this->m_ConvolutionTexture->Shutdown();
		delete this->m_ConvolutionTexture;
	}
	this->m_ConvolutionTexture = new RenderTextureClass;
	this->m_ConvolutionTexture->Initialize(d3d->GetDevice(), width, height, SCREEN_DEPTH, SCREEN_NEAR);
}

void ConvolutionEffect::RenderEffect(PostProcessor* processor, D3DClass* direct, const D3DXMATRIX& world, const D3DXMATRIX& view) const
{
	for (int i = 0; i < this->m_Iterations; i++)
	{
		this->RenderConvolution(processor, direct, world, view);
		PostEffect::CompleteRenderEffect(processor, direct, this->m_ConvolutionTexture, world, view);
	}
}

void ConvolutionEffect::RenderConvolution(PostProcessor* processor, D3DClass* direct, const D3DXMATRIX& world, const D3DXMATRIX& view) const
{
	float width = float(this->m_ConvolutionTexture->GetTextureWidth());
	float height = float(this->m_ConvolutionTexture->GetTextureHeight());

	this->m_ConvolutionTexture->SetRenderTarget(direct->GetDeviceContext());
	this->m_ConvolutionTexture->ClearRenderTarget(direct->GetDeviceContext(), 0.0f, 1.0f, 0.0f, 1.0f);

	D3DXMATRIX orthoMatrix;
	this->m_ConvolutionTexture->GetOrthoMatrix(orthoMatrix);

	processor->GetWindow()->Render(direct->GetDeviceContext());
	this->m_ConvolutionShader->Render(direct->GetDeviceContext(), processor->GetWindow()->GetIndexCount(), world, view,
		orthoMatrix, this->m_ConvolutionMatrix, processor->GetSceneRenderTexture()->GetShaderResourceView(), width, height);
}
