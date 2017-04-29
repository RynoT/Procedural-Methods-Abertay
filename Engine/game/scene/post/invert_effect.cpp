#include "invert_effect.h"

#include "post_processor.h"
#include "../../../orthowindowclass.h"
#include "../../../applicationclass.h"
#include "../../../invertshaderclass.h"
#include "../../../rendertextureclass.h"

InvertEffect::InvertEffect(D3DClass* d3d, const HWND& hwnd) : PostEffect(d3d, hwnd), m_InvertTexure(nullptr)
{
	this->m_InvertShader = new InvertShaderClass;
	this->m_InvertShader->Initialize(d3d->GetDevice(), hwnd);
}

InvertEffect::~InvertEffect()
{
	if(this->m_InvertTexure != nullptr)
	{
		this->m_InvertTexure->Shutdown();
		delete this->m_InvertTexure;
		this->m_InvertTexure = nullptr;
	}
	if(this->m_InvertShader != nullptr)
	{
		this->m_InvertShader->Shutdown();
		delete this->m_InvertShader;
		this->m_InvertShader = nullptr;
	}
}

void InvertEffect::OnResize(D3DClass* d3d, const int& width, const int& height)
{
	if (this->m_InvertTexure != nullptr)
	{
		this->m_InvertTexure->Shutdown();
		delete this->m_InvertTexure;
	}
	this->m_InvertTexure = new RenderTextureClass;
	this->m_InvertTexure->Initialize(d3d->GetDevice(), width, height, SCREEN_DEPTH, SCREEN_NEAR);

	PostEffect::OnResize(d3d, width, height);
}

void InvertEffect::RenderEffect(PostProcessor* processor, D3DClass* direct, const D3DXMATRIX& world, const D3DXMATRIX& view) const
{
	this->m_InvertTexure->SetRenderTarget(direct->GetDeviceContext());
	this->m_InvertTexure->ClearRenderTarget(direct->GetDeviceContext(), 0.0f, 1.0f, 0.0f, 1.0f);

	D3DXMATRIX orthoMatrix;
	this->m_InvertTexure->GetOrthoMatrix(orthoMatrix);

	processor->GetWindow()->Render(direct->GetDeviceContext());
	this->m_InvertShader->Render(direct->GetDeviceContext(), processor->GetWindow()->GetIndexCount(),
		world, view, orthoMatrix, processor->GetSceneRenderTexture()->GetShaderResourceView());

	PostEffect::CompleteRenderEffect(processor, direct, this->m_InvertTexure, world, view);
}
