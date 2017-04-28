#pragma once

#include "../../../d3dclass.h"

class PostProcessor;
class RenderTextureClass;

class PostEffect
{
public:
	PostEffect(D3DClass* d3d, const HWND& hwnd);
	virtual ~PostEffect() { }

	virtual void OnResize(D3DClass *d3d, const int& width, const int& height) { }

	virtual void Update(const float& delta) { }

	bool Render(PostProcessor *processor, D3DClass* direct) const;

	inline PostEffect* SetEnabled(const bool& enabled) { this->m_bEnabled = enabled; return this; }

protected:
	virtual void CompleteRenderEffect(PostProcessor *processor, D3DClass* direct, RenderTextureClass *texture, const D3DXMATRIX& world, const D3DXMATRIX& view) const;

	virtual void RenderEffect(PostProcessor *processor, D3DClass* direct) const = 0;

private:
	bool m_bEnabled;
};
