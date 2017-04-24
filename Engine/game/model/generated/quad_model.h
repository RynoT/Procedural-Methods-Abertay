#pragma once

#include "../model.h"

class QuadModel : public Model
{
public:
	QuadModel(const float& r, const float& g, const float& b, const float& a = 1.0f);

	virtual bool Initialize(ID3D11Device *device) override;

	inline const float& GetRed() const { return this->m_R; }

	inline const float& GetGreen() const { return this->m_G; }

	inline const float& GetBlue() const { return this->m_B; }

	inline const float& GetAlpha() const { return this->m_A; }

private:
	float m_R, m_G, m_B, m_A;
};
