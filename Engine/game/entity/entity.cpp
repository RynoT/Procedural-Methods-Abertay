#include "entity.h"

Entity::Entity() : m_bModelDirty(true), m_Position(0.0f), m_Scale(1.0f), m_Rotation(0.0f)
{
}

const D3DXMATRIX& Entity::GetModelMatrix()
{
	if(this->m_bModelDirty)
	{
		D3DXMatrixIdentity(&this->m_ModelMatrix);

		D3DXMATRIX transform;
		D3DXMatrixScaling(&transform, this->m_Scale.x, this->m_Scale.y, this->m_Scale.z);
		D3DXMatrixMultiply(&this->m_ModelMatrix, &this->m_ModelMatrix, &transform);

		if (this->m_Rotation.z != 0.0f)
		{
			D3DXMatrixRotationZ(&transform, this->m_Rotation.z);
			D3DXMatrixMultiply(&this->m_ModelMatrix, &this->m_ModelMatrix, &transform);
		}
		if (this->m_Rotation.x != 0.0f)
		{
			D3DXMatrixRotationX(&transform, this->m_Rotation.x);
			D3DXMatrixMultiply(&this->m_ModelMatrix, &this->m_ModelMatrix, &transform);
		}
		if (this->m_Rotation.y != 0.0f)
		{
			D3DXMatrixRotationY(&transform, this->m_Rotation.y);
			D3DXMatrixMultiply(&this->m_ModelMatrix, &this->m_ModelMatrix, &transform);
		}

		D3DXMatrixTranslation(&transform, this->m_Position.x, this->m_Position.y, this->m_Position.z);
		D3DXMatrixMultiply(&this->m_ModelMatrix, &this->m_ModelMatrix, &transform);

		this->m_bModelDirty = false;
	}
	return this->m_ModelMatrix;
}