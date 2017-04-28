#include "no_effect.h"

#include "post_processor.h"

NoEffect::NoEffect(D3DClass* d3d, const HWND& hwnd) : PostEffect(d3d, hwnd)
{
}

void NoEffect::RenderEffect(PostProcessor* processor, D3DClass* direct) const
{
	D3DXMATRIX world;
	direct->GetWorldMatrix(world);

	PostEffect::CompleteRenderEffect(processor, direct, processor->GetSceneRenderTexture(), world, processor->GetWindowViewMatrix());
}
