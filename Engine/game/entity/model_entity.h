#pragma once

#include "entity.h"

#include <functional>

class Model;
class D3DClass;

// Direct3D, projection, view, model
typedef std::function<void(D3DClass*, const D3DXMATRIX&, const D3DXMATRIX&, const D3DXMATRIX&)> RenderMethod;

class ModelEntity : public Entity
{
public:
	ModelEntity();
	virtual ~ModelEntity();

	virtual void Update(const float& delta) override { }

	virtual void Render(D3DClass *direct, const D3DXMATRIX& projection, const D3DXMATRIX& view);

	inline void SetModel(Model *model) { this->m_Model = model; }

	inline Model* GetInternalModel() const { return this->m_Model; }

	// The render method must be set for every entity. This method should call the shader.
	inline void SetRenderMethod(RenderMethod method) { this->m_RenderMethod = method; }

private:
	Model *m_Model;
	RenderMethod m_RenderMethod;
};
