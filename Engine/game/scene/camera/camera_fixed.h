#pragma once

#include "camera.h"

/// CameraFixed is a camera which works based off two points (A and B)
class CameraFixed : public Camera
{
public:
	CameraFixed();

	void Update(const float& delta) override;

	inline const Vector3f& GetUp() const override { return this->m_Up; }
	inline const Vector3f& GetLookAt() const override { return this->m_LookAt; }

	inline const Vector3f& GetPointA() const { return Camera::GetPosition(); }
	inline const Vector3f& GetPointB() const { return this->m_LookAt; }
	
	inline void SetPointA(const Vector3f& point) { Camera::SetPosition(point.x, point.y, point.z); Camera::SetViewAsDirty(); }
	inline void SetPointB(const Vector3f& point) { this->m_LookAt.set(point); Camera::SetViewAsDirty(); }

	inline void SetPoints(const Camera& camera) { this->SetPoints(camera.GetPosition(), camera.GetLookAt()); } //set both points based on existing camera
	inline void SetPoints(const Vector3f& pointA, const Vector3f& pointB) { this->SetPointA(pointA); this->SetPointB(pointB); } //set both points

private:
	Vector3f m_Up, m_LookAt;
};