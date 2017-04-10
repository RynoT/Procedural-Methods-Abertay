#pragma once

#include "entity.h"

#include <functional>

class D3DClass;
class ModelClass;

// Direct3D, projection, view, model
typedef std::function<void(D3DClass*, const D3DXMATRIX&, const D3DXMATRIX&, const D3DXMATRIX&)> RenderMethod;

class ModelEntity : public Entity
{
public:
	ModelEntity();
	virtual ~ModelEntity();

	bool SetFrom(ID3D11Device *device, char *modelPath, WCHAR *texturePath);

	virtual void Update(const float& delta) override { }

	virtual void Render(D3DClass *direct, const D3DXMATRIX& projection, const D3DXMATRIX& view);

	inline ModelClass* GetInternalModelClass() const { return this->m_Model; }

	inline void SetRenderMethod(RenderMethod method) { this->m_RenderMethod = method; }

private:
	ModelClass *m_Model;
	RenderMethod m_RenderMethod;
};
