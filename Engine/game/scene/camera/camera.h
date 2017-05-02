#pragma once

#include "../../util/math/vector3f.h"

// Abstract class representing a camera 
class Camera
{
public:
	Camera();
	virtual ~Camera() { }

	virtual void Update(const float& delta) = 0;

	virtual const D3DXMATRIX& GetViewMatrix();

	virtual const Vector3f& GetUp() const = 0;
	virtual const Vector3f& GetLookAt() const = 0;

	inline const float& GetX() const { return this->m_Position.x; }

	inline const float& GetY() const { return this->m_Position.y; }

	inline const float& GetZ() const { return this->m_Position.z; }

	inline const Vector3f& GetPosition() const { return this->m_Position; }

	virtual void SetPosition(const float& x, const float& y, const float& z)
	{
		this->SetViewAsDirty();
		this->m_Position.set(x, y, z); 
	}

	virtual void Translate(const float& dx, const float& dy, const float& dz)
	{
		this->SetPosition(this->m_Position.x + dx, this->m_Position.y + dy, this->m_Position.z + dz);
	}

	inline const Vector3f& operator+=(const Vector3f& vector) { this->Translate(vector.x, vector.y, vector.z); return this->m_Position; }

protected:
	// Let the camera know that the view matrix needs to be reconstructed
	inline void SetViewAsDirty() { this->m_bViewDirty = true; }

private:
	Vector3f m_Position;

	bool m_bViewDirty;
	D3DXMATRIX m_ViewMatrix;
};