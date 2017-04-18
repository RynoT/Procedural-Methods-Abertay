#include "camera_fpv.h"

#include <math.h>

CameraFPV::CameraFPV(const float& sensitivity, const bool& invertX, const bool& invertY)
	: m_Yaw(0.0f), m_Pitch(0.0f), m_Roll(0.0f), m_Sensitivity(sensitivity), m_bInvertX(invertX), m_bInvertY(invertY)
{
}

void CameraFPV::Rotate(float dx, float dy)
{
	if (this->m_bInvertX)
	{
		dx = -dx;
	}
	if (this->m_bInvertY)
	{
		dy = -dy;
	}
	this->AddYaw(dx * this->m_Sensitivity);
	this->AddPitch(-dy * this->m_Sensitivity);
}

void CameraFPV::MoveVertical(const float& amount)
{
	Camera::Translate(0.0f, amount, 0.0f);
}
  
void CameraFPV::MoveForward(const float& amount)
{
	Camera::Translate(VECTOR3_SPLIT(this->m_Forward * amount));
}

void CameraFPV::MoveSideways(const float& amount) //V(a, b), Vperp(-b, a). We only move sideways on the X-Z axis. (Where y-axis is 'up and down' or height)
{
	Camera::Translate(-this->m_Forward.z * amount, 0.0f, this->m_Forward.x * amount);
}

void CameraFPV::Update(const float& delta)
{
	float cosR, cosP, cosY;	//temp values for sin/cos from 
	float sinR, sinP, sinY;

	//since this is a first-person camera, don't allow the player to look too far up or down.
	//if (this->m_Pitch > 90.0f)
	//{
	//	this->m_Pitch = 90.0f;
	//}
	//else if (this->m_Pitch < -90.0f)
	//{
	//	this->m_Pitch = -90.0f;
	//}

	const float radians = 3.14159265358979f / 180.0f;
	cosP = cosf(this->m_Pitch * radians);
	cosY = cosf(this->m_Yaw * radians);
	cosR = cosf(this->m_Roll * radians);
	sinP = sinf(this->m_Pitch * radians);
	sinY = sinf(this->m_Yaw * radians);
	sinR = sinf(this->m_Roll * radians);

	this->m_Forward.set(sinY * cosP, sinP, cosP * -cosY);
	this->m_Up.set(-cosY * sinR - sinY * sinP * cosR,
		cosP * cosR, -sinY * sinR - sinP * cosR * -cosY);

	this->m_LookAt.set(Camera::GetPosition() + this->m_Forward);
}