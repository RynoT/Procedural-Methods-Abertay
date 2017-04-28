#include "blur_effect.h"

#include "post_processor.h"
#include "../../../applicationclass.h"
#include "../../../orthowindowclass.h"
#include "../../../rendertextureclass.h"
#include "../../../textureshaderclass.h"
#include "../../../verticalblurshaderclass.h"
#include "../../../horizontalblurshaderclass.h"

BlurEffect::BlurEffect(D3DClass *d3d, const HWND& hwnd) : PostEffect(d3d, hwnd), m_VerticalBlurShader(nullptr), m_HorizontalBlurShader(nullptr),
m_DownSampleTexure(nullptr), m_HorizontalBlurTexture(nullptr), m_VerticalBlurTexture(nullptr), m_UpSampleTexure(nullptr)
{
	this->m_HorizontalBlurShader = new HorizontalBlurShaderClass;
	this->m_HorizontalBlurShader->Initialize(d3d->GetDevice(), hwnd);

	this->m_VerticalBlurShader = new VerticalBlurShaderClass;
	this->m_VerticalBlurShader->Initialize(d3d->GetDevice(), hwnd);

	this->BlurEffect::OnResize(d3d, ApplicationClass::SCREEN_WIDTH, ApplicationClass::SCREEN_HEIGHT);
}

BlurEffect::~BlurEffect()
{
	if (this->m_UpSampleTexure != nullptr)
	{
		this->m_UpSampleTexure->Shutdown();
		delete this->m_UpSampleTexure;
		this->m_UpSampleTexure = nullptr;
	}
	if (this->m_VerticalBlurTexture != nullptr)
	{
		this->m_VerticalBlurTexture->Shutdown();
		delete this->m_VerticalBlurTexture;
		this->m_VerticalBlurTexture = nullptr;
	}
	if (this->m_HorizontalBlurTexture != nullptr)
	{
		this->m_HorizontalBlurTexture->Shutdown();
		delete this->m_HorizontalBlurTexture;
		this->m_HorizontalBlurTexture = nullptr;
	}
	if (this->m_DownSampleTexure != nullptr)
	{
		this->m_DownSampleTexure->Shutdown();
		delete this->m_DownSampleTexure;
		this->m_DownSampleTexure = nullptr;
	}
	if (this->m_VerticalBlurShader != nullptr)
	{
		this->m_VerticalBlurShader->Shutdown();
		delete this->m_VerticalBlurShader;
		this->m_VerticalBlurShader = nullptr;
	}
	if (this->m_HorizontalBlurShader != nullptr)
	{
		this->m_HorizontalBlurShader->Shutdown();
		delete this->m_HorizontalBlurShader;
		this->m_HorizontalBlurShader = nullptr;
	}
}

void BlurEffect::OnResize(D3DClass* d3d, const int& width, const int& height)
{
	if (this->m_DownSampleTexure != nullptr)
	{
		this->m_DownSampleTexure->Shutdown();
		delete this->m_DownSampleTexure;
	}
	if (this->m_HorizontalBlurTexture != nullptr)
	{
		this->m_HorizontalBlurTexture->Shutdown();
		delete this->m_HorizontalBlurTexture;
	}
	if (this->m_VerticalBlurTexture != nullptr)
	{
		this->m_VerticalBlurTexture->Shutdown();
		delete this->m_VerticalBlurTexture;
	}
	if (this->m_UpSampleTexure != nullptr)
	{
		this->m_UpSampleTexure->Shutdown();
		delete this->m_UpSampleTexure;
	}

	this->m_DownSampleTexure = new RenderTextureClass;
	this->m_DownSampleTexure->Initialize(d3d->GetDevice(),
		DOWN_SAMPLE_WIDTH, DOWN_SAMPLE_HEIGHT, SCREEN_DEPTH, SCREEN_NEAR);

	this->m_HorizontalBlurTexture = new RenderTextureClass;
	this->m_HorizontalBlurTexture->Initialize(d3d->GetDevice(),
		DOWN_SAMPLE_WIDTH, DOWN_SAMPLE_HEIGHT, SCREEN_DEPTH, SCREEN_NEAR);

	this->m_VerticalBlurTexture = new RenderTextureClass;
	this->m_VerticalBlurTexture->Initialize(d3d->GetDevice(),
		DOWN_SAMPLE_WIDTH, DOWN_SAMPLE_HEIGHT, SCREEN_DEPTH, SCREEN_NEAR);

	this->m_UpSampleTexure = new RenderTextureClass;
	this->m_UpSampleTexure->Initialize(d3d->GetDevice(), width, height, SCREEN_DEPTH, SCREEN_NEAR);
}

void BlurEffect::RenderEffect(PostProcessor* processor, D3DClass* direct) const
{
	D3DXMATRIX world, view = processor->GetWindowViewMatrix();
	direct->GetWorldMatrix(world);

	direct->TurnZBufferOff();

	this->RenderDownSample(processor, direct, world, view);
	this->RenderHorizontalSample(processor, direct, world, view);
	this->RenderVerticalSample(processor, direct, world, view);
	this->RenderUpSample(processor, direct, world, view);

	PostEffect::CompleteRenderEffect(processor, direct, this->m_UpSampleTexure, world, view);
}

void BlurEffect::RenderUpSample(PostProcessor* processor, D3DClass* direct, const D3DXMATRIX& world, const D3DXMATRIX& view) const
{
	this->m_UpSampleTexure->SetRenderTarget(direct->GetDeviceContext());
	this->m_UpSampleTexure->ClearRenderTarget(direct->GetDeviceContext(), 0.0f, 1.0f, 0.0f, 1.0f);

	D3DXMATRIX orthoMatrix;
	this->m_UpSampleTexure->GetOrthoMatrix(orthoMatrix);

	processor->GetWindow()->Render(direct->GetDeviceContext());
	processor->GetTextureShader()->Render(direct->GetDeviceContext(), processor->GetWindow()->GetIndexCount(),
		world, view, orthoMatrix, this->m_VerticalBlurTexture->GetShaderResourceView());
}

void BlurEffect::RenderVerticalSample(PostProcessor* processor, D3DClass* direct, const D3DXMATRIX& world, const D3DXMATRIX& view) const
{
	this->m_VerticalBlurTexture->SetRenderTarget(direct->GetDeviceContext());
	this->m_VerticalBlurTexture->ClearRenderTarget(direct->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	D3DXMATRIX orthoMatrix;
	this->m_VerticalBlurTexture->GetOrthoMatrix(orthoMatrix);

	processor->GetSmallWindow()->Render(direct->GetDeviceContext());
	this->m_VerticalBlurShader->Render(direct->GetDeviceContext(), processor->GetSmallWindow()->GetIndexCount(), world, view, orthoMatrix,
		this->m_HorizontalBlurTexture->GetShaderResourceView(), float(this->m_VerticalBlurTexture->GetTextureHeight()));
}

void BlurEffect::RenderHorizontalSample(PostProcessor* processor, D3DClass* direct, const D3DXMATRIX& world, const D3DXMATRIX& view) const
{
	this->m_HorizontalBlurTexture->SetRenderTarget(direct->GetDeviceContext());
	this->m_HorizontalBlurTexture->ClearRenderTarget(direct->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	D3DXMATRIX orthoMatrix;
	this->m_HorizontalBlurTexture->GetOrthoMatrix(orthoMatrix);

	processor->GetSmallWindow()->Render(direct->GetDeviceContext());
	this->m_HorizontalBlurShader->Render(direct->GetDeviceContext(), processor->GetSmallWindow()->GetIndexCount(), world, view, orthoMatrix,
		this->m_DownSampleTexure->GetShaderResourceView(), float(this->m_HorizontalBlurTexture->GetTextureWidth()));
}

void BlurEffect::RenderDownSample(PostProcessor* processor, D3DClass* direct, const D3DXMATRIX& world, const D3DXMATRIX& view) const
{
	this->m_DownSampleTexure->SetRenderTarget(direct->GetDeviceContext());
	this->m_DownSampleTexure->ClearRenderTarget(direct->GetDeviceContext(), 0.0f, 1.0f, 0.0f, 1.0f);

	D3DXMATRIX orthoMatrix;
	this->m_DownSampleTexure->GetOrthoMatrix(orthoMatrix);

	processor->GetSmallWindow()->Render(direct->GetDeviceContext());
	processor->GetTextureShader()->Render(direct->GetDeviceContext(), processor->GetSmallWindow()->GetIndexCount(),
		world, view, orthoMatrix, processor->GetSceneRenderTexture()->GetShaderResourceView());
}
