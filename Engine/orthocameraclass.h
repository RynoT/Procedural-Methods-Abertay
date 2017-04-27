////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_


//////////////
// INCLUDES //
//////////////
#include <D3DX10.h>

////////////////////////////////////////////////////////////////////////////////
// Class name: OrthoCameraClass
////////////////////////////////////////////////////////////////////////////////
class OrthoCameraClass
{
public:
	OrthoCameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	D3DXVECTOR3 GetPosition() const;
	D3DXVECTOR3 GetRotation() const;

	void Render();
	void GetViewMatrix(D3DXMATRIX&) const;

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	D3DXMATRIX m_viewMatrix;
};

#endif