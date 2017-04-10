#pragma once

#include "camera.h"

class CameraAxis : public Camera
{
public:
	CameraAxis(const Vector3f& right = Vector3f(1.0f, 0.0f, 0.0f), const Vector3f& forward = Vector3f(0.0f, 0.0f, 1.0f));

	virtual void Update() override { }

	virtual void Translate(const float& dright, const float& dup, const float& dforward) override;

	inline const Vector3f& GetUp() const override { return this->m_Up; }
	inline const Vector3f& GetLookAt() const override { return this->m_LookAt; }

private:
	Vector3f m_Up, m_LookAt;

	Vector3f m_Right, m_Forward;
};
