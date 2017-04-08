#include "entity.h"

Entity::Entity() : m_bModelDirty(true), m_Position(0.0f, 0.0f, 0.0f), m_Rotation(0.0f, 0.0f, 0.0f)
{
}

const D3DXMATRIX& Entity::GetModelMatrix()
{
	if(this->m_bModelDirty)
	{
		D3DXMatrixIdentity(&this->m_ModelMatrix);
		D3DXMatrixTranslation(&this->m_ModelMatrix, this->m_Position.x, this->m_Position.y, this->m_Position.z);
		if (this->m_Rotation.z != 0.0f)
		{
			D3DXMatrixRotationZ(&this->m_ModelMatrix, this->m_Rotation.z);
		}
		if (this->m_Rotation.x != 0.0f)
		{
			D3DXMatrixRotationX(&this->m_ModelMatrix, this->m_Rotation.x);
		}
		if (this->m_Rotation.y != 0.0f)
		{
			D3DXMatrixRotationY(&this->m_ModelMatrix, this->m_Rotation.y);
		}
		this->m_bModelDirty = false;
	}
	return this->m_ModelMatrix;
}