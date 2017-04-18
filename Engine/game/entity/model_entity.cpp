#include "model_entity.h"

#include "../model/model.h"
#include "../../d3dclass.h"

ModelEntity::ModelEntity() : m_Model(nullptr)//, m_RenderMethod(nullptr)
{
}

ModelEntity::~ModelEntity()
{
	if(this->m_Model != nullptr)
	{
		delete this->m_Model;
		this->m_Model = nullptr;
	}
}

void ModelEntity::Render(D3DClass* direct, const D3DXMATRIX& projection, const D3DXMATRIX& view)
{
	if(this->m_Model == nullptr || this->m_RenderMethod == nullptr)
	{
		return;
	}
	const D3DXMATRIX& model = Entity::GetModelMatrix();
	this->m_Model->Render(direct->GetDeviceContext());
	this->m_RenderMethod(direct, projection, view, model);
}
