#include "post_effect.h"

#include "post_processor.h"
#include "../../../applicationclass.h"
#include "../../../orthowindowclass.h"
#include "../../../rendertextureclass.h"
#include "../../../textureshaderclass.h"

PostEffect::PostEffect(D3DClass* d3d, const HWND& hwnd) : m_bEnabled(true)
{
}

bool PostEffect::Render(PostProcessor* processor, D3DClass* direct) const
{
	if(!this->m_bEnabled)
	{
		return false;
	}
	this->RenderEffect(processor, direct);
	return true;
}

void PostEffect::CompleteRenderEffect(PostProcessor* processor, D3DClass* direct, RenderTextureClass* texture, const D3DXMATRIX& world, const D3DXMATRIX& view) const
{
	direct->SetBackBufferRenderTarget();
	direct->ResetViewport();

	D3DXMATRIX orthoMatrix;
	direct->GetOrthoMatrix(orthoMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	direct->TurnZBufferOff();

	// Put the full screen ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing.
	processor->GetWindow()->Render(direct->GetDeviceContext());

	// Render the full screen ortho window using the texture shader and the full screen sized blurred render to texture resource.
	processor->GetTextureShader()->Render(direct->GetDeviceContext(), processor->GetWindow()->GetIndexCount(),
		world, view, orthoMatrix, texture->GetShaderResourceView());

	// Turn the Z buffer back on now that all 2D rendering has completed.
	direct->TurnZBufferOn();
}
