#pragma once

#include "../../d3dclass.h"

class Entity
{
protected:
	Entity();

public:
	virtual ~Entity() { }

	const D3DXMATRIX& GetModelMatrix();

	virtual void Update(const float& delta) = 0;

	inline const D3DXVECTOR3& GetPosition() const { return this->m_Position; }

	inline const D3DXVECTOR3& GetRotation() const { return this->m_Rotation; }

	inline void SetPosition(const float& x, const float& y, const float& z) 
	{ 
		this->m_bModelDirty = true;
		this->m_Position.x = x; this->m_Position.y = y; this->m_Position.z = z;
	}

	inline void SetRotation(const float& x, const float& y, const float& z)
	{
		this->m_bModelDirty = true;
		this->m_Rotation.x = x; this->m_Rotation.y = y; this->m_Rotation.z = z;
	}

private:
	bool m_bModelDirty;
	D3DXMATRIX m_ModelMatrix;

	D3DXVECTOR3 m_Position, m_Rotation;
};
