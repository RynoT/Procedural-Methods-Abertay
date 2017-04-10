#include "camera_axis.h"

CameraAxis::CameraAxis(const Vector3f& right, const Vector3f& forward) : m_Right(right), m_Forward(forward)
{
	this->m_LookAt.set(forward);
	this->m_Up.set(forward.cross(right));
}

void CameraAxis::Translate(const float& dright, const float& dup, const float& dforward)
{
	Vector3f out = (this->m_Right * dright) + (this->m_Up * dup) + (this->m_Forward * dforward);
	Camera::Translate(VECTOR3_SPLIT(out));
	this->m_LookAt.set(Camera::GetPosition() + this->m_Forward);
}