#pragma once

#include "post_effect.h"

class NoEffect : public PostEffect
{
public:
	NoEffect(D3DClass* d3d, const HWND& hwnd);

protected:
	virtual void RenderEffect(PostProcessor* processor, D3DClass* direct) const override;
};