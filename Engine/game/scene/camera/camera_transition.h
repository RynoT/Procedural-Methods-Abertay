#pragma once

#include "camera_fpv.h"

class CameraTransition : public CameraFPV
{
public:
	CameraTransition(const Vector3f& startPos, const Vector3f& startRot);
	CameraTransition(const Vector3f& startPos, const Vector3f& startRot, 
		const Vector3f& endPos, const Vector3f& endRot, const float& duration);

	void Transition(const Vector3f& position, const Vector3f& rotation, const float& duration);

	virtual void Update(const float& delta) override;

	bool IsTransitionComplete() const { return this->m_Counter >= this->m_Duration; }

private:
	float m_Duration, m_Counter;
	Vector3f m_StartPos, m_EndPos, m_StartRot, m_EndRot;
};
