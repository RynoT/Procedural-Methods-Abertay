#pragma once

#include "entity.h"

#include <functional>

class D3DClass;
class ModelClass;

class ModelEntity : public Entity
{
public:
	ModelEntity();
	virtual ~ModelEntity();

	bool SetFrom(ID3D11Device *device, char *modelPath, WCHAR *texturePath);

	virtual void Update(const float& delta) override { }

	virtual void Render(D3DClass *direct, const  D3DXMATRIX& projection, const D3DXMATRIX& view);

	inline void SetRenderMethod(std::function<void(ModelEntity*)> method) { this->m_RenderMethod = method; }

private:
	ModelClass *m_Model;
	std::function<void(ModelEntity*)> m_RenderMethod;
};
