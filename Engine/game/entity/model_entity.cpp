#include "model_entity.h"

#include "../../d3dclass.h"
#include "../../modelclass.h"

ModelEntity::ModelEntity() : m_Model(nullptr)//, m_RenderMethod(nullptr)
{
}

ModelEntity::~ModelEntity()
{
	if(this->m_Model != nullptr)
	{
		this->m_Model->Shutdown();
		delete this->m_Model;
		this->m_Model = nullptr;
	}
}

bool ModelEntity::SetFrom(ID3D11Device* device, char* modelPath, WCHAR* texturePath)
{
	if(this->m_Model == nullptr)
	{
		this->m_Model = new ModelClass;
	}
	return this->m_Model->Initialize(device, modelPath, texturePath);
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
