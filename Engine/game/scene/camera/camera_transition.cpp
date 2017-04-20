#include "camera_transition.h"

#include "../../../systemclass.h"

#include <cassert>

CameraTransition::CameraTransition(const Vector3f& startPos, const Vector3f& endPos, const Vector3f& rotation, const float& duration)
	: m_Duration(duration), m_Counter(0.0f), m_StartPos(startPos), m_EndPos(endPos), m_Rotation(rotation)
{
	CameraFPV::SetRoll(180.0f);
	CameraFPV::SetPitch(180.0f);
	Camera::SetPosition(VECTOR3_SPLIT(startPos));
}

void CameraTransition::Update(const float& delta)
{
	if (this->m_Duration == 0.0f)
	{
		assert(false);
	}
	this->m_Counter += delta;

	if(!this->IsTransitionComplete())
	{
		float percentage = sinf((3.14159f / 2.0f) * min(this->m_Counter / this->m_Duration, 1.0f));

		Vector3f position = this->m_StartPos + (this->m_EndPos - this->m_StartPos) * percentage;
		Camera::SetPosition(VECTOR3_SPLIT(position));

		CameraFPV::SetYaw(this->m_Rotation.y * percentage);
		CameraFPV::SetPitch(180.0f + (this->m_Rotation.x - 180.0f) * percentage);
		CameraFPV::SetRoll(180.0f + (this->m_Rotation.z - 180.0f) * percentage);
	}

	CameraFPV::Update(delta);
}
