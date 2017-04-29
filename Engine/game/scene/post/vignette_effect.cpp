#include "vignette_effect.h"

#include "post_processor.h"
#include "../../../orthowindowclass.h"
#include "../../../applicationclass.h"
#include "../../../rendertextureclass.h"
#include "../../../vignetteshaderclass.h"

VignetteEffect::VignetteEffect(D3DClass* d3d, const HWND& hwnd) : PostEffect(d3d, hwnd), m_VignetteTexture(nullptr)
{
	this->m_VignetteShader = new VignetteShaderClass;
	this->m_VignetteShader->Initialize(d3d->GetDevice(), hwnd);
}

VignetteEffect::~VignetteEffect()
{
	if(this->m_VignetteTexture != nullptr)
	{
		this->m_VignetteTexture->Shutdown();
		delete this->m_VignetteTexture;
		this->m_VignetteTexture = nullptr;
	}
	if(this->m_VignetteShader != nullptr)
	{
		this->m_VignetteShader->Shutdown();
		delete this->m_VignetteShader;
		this->m_VignetteShader = nullptr;
	}
}

void VignetteEffect::OnResize(D3DClass* d3d, const int& width, const int& height)
{
	if (this->m_VignetteTexture != nullptr)
	{
		this->m_VignetteTexture->Shutdown();
		delete this->m_VignetteTexture;
	}
	this->m_VignetteTexture = new RenderTextureClass;
	this->m_VignetteTexture->Initialize(d3d->GetDevice(), width, height, SCREEN_DEPTH, SCREEN_NEAR);

	PostEffect::OnResize(d3d, width, height);
}

void VignetteEffect::RenderEffect(PostProcessor* processor, D3DClass* direct, const D3DXMATRIX& world, const D3DXMATRIX& view) const
{
	this->m_VignetteTexture->SetRenderTarget(direct->GetDeviceContext());
	this->m_VignetteTexture->ClearRenderTarget(direct->GetDeviceContext(), 0.0f, 1.0f, 0.0f, 1.0f);

	D3DXMATRIX orthoMatrix;
	this->m_VignetteTexture->GetOrthoMatrix(orthoMatrix);

	processor->GetWindow()->Render(direct->GetDeviceContext());
	this->m_VignetteShader->Render(direct->GetDeviceContext(), processor->GetWindow()->GetIndexCount(),
		world, view, orthoMatrix, processor->GetSceneRenderTexture()->GetShaderResourceView());

	PostEffect::CompleteRenderEffect(processor, direct, this->m_VignetteTexture, world, view);
}
