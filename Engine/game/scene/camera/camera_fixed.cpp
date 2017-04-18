#include "camera_fixed.h"

CameraFixed::CameraFixed() : m_Up(0.0f, 1.0f, 0.0f) //up is always directly up on y axis
{
}

void CameraFixed::Update(const float& delta)
{
	//this->ray.set(this->lookAt.getX() - Camera::position.getX(), this->lookAt.getY() 
	//	- Camera::position.getY(), this->lookAt.getZ() - Camera::position.getZ());
	//this->up.set(0.0f, 1.0f, 0.0f);
}
