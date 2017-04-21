#include "camera.h"

Camera::Camera() : m_bViewDirty(true)
{
}

const D3DXMATRIX& Camera::GetViewMatrix()
{
	if (this->m_bViewDirty)
	{
		D3DXVECTOR3 position = this->GetPosition(), lookAt = this->GetLookAt(), up = this->GetUp();
		D3DXMatrixIdentity(&this->m_ViewMatrix);
		D3DXMatrixLookAtLH(&this->m_ViewMatrix, &position, &lookAt, &up);

		this->m_bViewDirty = false;
	}
	return this->m_ViewMatrix;
}
