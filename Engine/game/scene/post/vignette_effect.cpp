#include "vignette_effect.h"

#include "post_processor.h"
#include "../../../orthowindowclass.h"
#include "../../../applicationclass.h"
#include "../../../textureshaderclass.h"
#include "../../../rendertextureclass.h"
#include "../../../vignetteshaderclass.h"

VignetteEffect::VignetteEffect(D3DClass* d3d, const HWND& hwnd) : PostEffect(d3d, hwnd), m_VignetteTexure(nullptr)
{
	this->m_VignetteShader = new VignetteShaderClass;
	this->m_VignetteShader->Initialize(d3d->GetDevice(), hwnd);
}

VignetteEffect::~VignetteEffect()
{
	if(this->m_VignetteTexure != nullptr)
	{
		this->m_VignetteTexure->Shutdown();
		delete this->m_VignetteTexure;
		this->m_VignetteTexure = nullptr;
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
	if (this->m_VignetteTexure != nullptr)
	{
		this->m_VignetteTexure->Shutdown();
		delete this->m_VignetteTexure;
	}
	this->m_VignetteTexure = new RenderTextureClass;
	this->m_VignetteTexure->Initialize(d3d->GetDevice(), width, height, SCREEN_DEPTH, SCREEN_NEAR);
}

void VignetteEffect::RenderEffect(PostProcessor* processor, D3DClass* direct, const D3DXMATRIX& world, const D3DXMATRIX& view) const
{
	this->m_VignetteTexure->SetRenderTarget(direct->GetDeviceContext());
	this->m_VignetteTexure->ClearRenderTarget(direct->GetDeviceContext(), 0.0f, 1.0f, 0.0f, 1.0f);

	D3DXMATRIX orthoMatrix;
	this->m_VignetteTexure->GetOrthoMatrix(orthoMatrix);

	processor->GetWindow()->Render(direct->GetDeviceContext());
	this->m_VignetteShader->Render(direct->GetDeviceContext(), processor->GetWindow()->GetIndexCount(),
		world, view, orthoMatrix, processor->GetSceneRenderTexture()->GetShaderResourceView());

	PostEffect::CompleteRenderEffect(processor, direct, this->m_VignetteTexure, world, view);
}
