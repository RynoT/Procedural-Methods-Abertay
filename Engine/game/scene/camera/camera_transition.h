#pragma once

#include "camera_fpv.h"

class CameraTransition : public CameraFPV
{
public:
	CameraTransition(const Vector3f& startPos, const Vector3f& startLook,
		const Vector3f& endPos, const Vector3f& endLook, const float& duration);

	virtual void Update(const float& delta) override;

	bool IsTransitionComplete() const { return this->m_Counter >= this->m_Duration; }

private:
	float m_Duration, m_Counter;
	Vector3f m_StartPos, m_StartLook, m_EndPos, m_EndLook;
};
