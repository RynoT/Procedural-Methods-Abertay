#include "camera_transition.h"

#include "../../../systemclass.h"

#include <cassert>

#define LERP(a, b, p) (a + (b - a) * p)

CameraTransition::CameraTransition(const Vector3f& startPos, const Vector3f& startRot) 
	: CameraTransition(startPos, startRot, startPos, startRot, 0.0f)
{
}

CameraTransition::CameraTransition(const Vector3f& startPos, const Vector3f& startRot, const Vector3f& endPos, const Vector3f& endRot, const float& duration)
	: m_Duration(duration), m_Counter(0.0f), m_StartPos(startPos), m_EndPos(endPos), m_StartRot(startRot), m_EndRot(endRot)
{
	CameraFPV::SetPitch(startRot.x);
	CameraFPV::SetYaw(startRot.y);
	CameraFPV::SetRoll(startRot.z);
	Camera::SetPosition(VECTOR3_SPLIT(startPos));
}

void CameraTransition::Transition(const Vector3f& position, const Vector3f& rotation, const float& duration)
{
	this->m_StartPos = Camera::GetPosition();
	this->m_StartRot = CameraFPV::GetYawPitchRoll();
	this->m_EndPos = position;
	this->m_EndRot = rotation;
	this->m_Duration = duration;
	this->m_Counter = 0.0f;
}

void CameraTransition::Update(const float& delta)
{
	this->m_Counter += delta;

	if(!this->IsTransitionComplete())
	{
		assert(this->m_Duration != 0.0f);
		float percentage = sinf((3.14159f / 2.0f) * min(this->m_Counter / this->m_Duration, 1.0f));

		Vector3f position = this->m_StartPos + (this->m_EndPos - this->m_StartPos) * percentage;
		Camera::SetPosition(VECTOR3_SPLIT(position));

		CameraFPV::SetPitch(LERP(this->m_StartRot.x, this->m_EndRot.x, percentage));
		CameraFPV::SetYaw(LERP(this->m_StartRot.y, this->m_EndRot.y, percentage));
		CameraFPV::SetRoll(LERP(this->m_StartRot.z, this->m_EndRot.z, percentage));
	}
	else
	{
		const Vector3f& currentPos = Camera::GetPosition();
		Camera::SetPosition(currentPos.x, this->m_EndPos.y, currentPos.z);
	}

	CameraFPV::Update(delta);
}
