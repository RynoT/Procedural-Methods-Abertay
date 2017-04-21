#pragma once

#include "camera.h"

#define CAMERA_FPV_INVERT_X false
#define CAMERA_FPV_INVERT_Y false
#define CAMERA_FPV_MOVE_SENSITIVITY 20.0f

/// First-person camera is a camera which works based off of yaw, pitch, and roll. Useful for first person movement.
class CameraFPV : public Camera //Camera First Person View
{
public:
	CameraFPV(const float& sensitivity = CAMERA_FPV_MOVE_SENSITIVITY,
		const bool& invertX = CAMERA_FPV_INVERT_X, const bool& invertY = CAMERA_FPV_INVERT_Y);

	virtual void Update(const float& delta) override;

	/// Rotate yaw and pitch (factors in camera sensitivity)
	void Rotate(float dx, float dy);

	void MoveForward(const float& amount); //move forward and backwards (according to camera rotation)
	void MoveSideways(const float& amount); //move left and right (according to cameras rotation - ignores y aixs)
	void MoveVertical(const float& amount); //move up and down (on y axis)

	inline void SetYaw(const float& yaw) { this->m_Yaw = yaw; }
	inline void SetPitch(const float& pitch) { this->m_Pitch = pitch; }
	inline void SetRoll(const float& roll) { this->m_Roll = roll; }

	inline void AddYaw(const float& yaw) { this->m_Yaw += yaw; Camera::SetViewAsDirty(); }
	inline void AddPitch(const float& pitch) { this->m_Pitch += pitch; Camera::SetViewAsDirty(); }
	inline void AddRoll(const float& roll) { this->m_Roll += roll; Camera::SetViewAsDirty(); }

	inline void SetInvertX(const bool& invert) { this->m_bInvertX = invert; }
	inline void SetInvertY(const bool& invert) { this->m_bInvertY = invert; }
	inline void SetSensitivity(const float& sensitivity) { this->m_Sensitivity = sensitivity; }

	inline const bool& IsInvertX() const { return this->m_bInvertX; }
	inline const bool& IsInvertY() const { return this->m_bInvertY; }
	inline const float& GetSensitivity() const { return this->m_Sensitivity; }

	inline const float& GetYaw() const { return this->m_Yaw; }
	inline const float& GetPitch() const { return this->m_Pitch; }
	inline const float& GetRoll() const { return this->m_Roll; }

	inline Vector3f GetYawPitchRoll() const { return Vector3f(this->m_Pitch, this->m_Yaw, this->m_Roll); }

	inline const Vector3f& GetUp() const override { return this->m_Up; }
	inline const Vector3f& GetLookAt() const override { return this->m_LookAt; }
	inline const Vector3f& GetForward() const { return this->m_Forward; }

private:
	Vector3f m_LookAt;
	Vector3f m_Forward, m_Up;

	float m_Yaw, m_Pitch, m_Roll;
	float m_Sensitivity;
	bool m_bInvertX, m_bInvertY;
};