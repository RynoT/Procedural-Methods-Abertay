#include "post_effect.h"

#include "post_processor.h"
#include "../../../applicationclass.h"
#include "../../../orthowindowclass.h"
#include "../../../rendertextureclass.h"
#include "../../../textureshaderclass.h"

PostEffect::PostEffect(D3DClass* d3d, const HWND& hwnd) : m_bEnabled(true)
{
}

bool PostEffect::Render(PostProcessor* processor, D3DClass* direct, const D3DXMATRIX& world, const D3DXMATRIX& view) const
{
	if(!this->m_bEnabled)
	{
		return false;
	}
	this->RenderEffect(processor, direct, world, view);
	return true;
}

void PostEffect::CompleteRenderEffect(PostProcessor* processor, D3DClass* direct, RenderTextureClass* texture, const D3DXMATRIX& world, const D3DXMATRIX& view) const
{
	processor->GetSceneRenderTexture()->SetRenderTarget(direct->GetDeviceContext());
	processor->GetSceneRenderTexture()->ClearRenderTarget(direct->GetDeviceContext(), 0.0f, 1.0f, 0.0f, 1.0f);

	D3DXMATRIX orthoMatrix;
	processor->GetSceneRenderTexture()->GetOrthoMatrix(orthoMatrix);

	processor->GetWindow()->Render(direct->GetDeviceContext());
	processor->GetTextureShader()->Render(direct->GetDeviceContext(), processor->GetWindow()->GetIndexCount(),
		world, view, orthoMatrix, texture->GetShaderResourceView());
}
