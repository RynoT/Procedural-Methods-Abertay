#pragma once

#include "../util/math/vector3f.h"

class Entity
{
protected:
	Entity();

public:
	virtual ~Entity() { }

	const D3DXMATRIX& GetModelMatrix();

	virtual void Update(const float& delta) = 0;

	inline const Vector3f& GetScale() const { return this->m_Scale; }

	inline const Vector3f& GetPosition() const { return this->m_Position; }

	inline const Vector3f& GetRotation() const { return this->m_Rotation; }

	inline void SetPosition(const float& x, const float& y, const float& z) 
	{ 
		this->m_bModelDirty = true;
		this->m_Position.x = x; this->m_Position.y = y; this->m_Position.z = z;
	}

	inline void SetScale(const float& xyz) { this->SetScale(xyz, xyz, xyz); }

	inline void SetScale(const float& x, const float& y, const float& z)
	{
		this->m_bModelDirty = true;
		this->m_Scale.x = x; this->m_Scale.y = y; this->m_Scale.z = z;
	}

	inline void SetRotation(const float& x, const float& y, const float& z)
	{
		this->m_bModelDirty = true;
		this->m_Rotation.x = x; this->m_Rotation.y = y; this->m_Rotation.z = z;
	}

private:
	bool m_bModelDirty;
	D3DXMATRIX m_ModelMatrix;

	Vector3f m_Position, m_Scale, m_Rotation;
};
