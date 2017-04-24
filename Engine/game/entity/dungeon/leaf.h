#pragma once

#include "../model_entity.h"

class QuadModel;
class ColorShaderClass;

class Leaf : public ModelEntity
{
public:
	Leaf(const float& x, const float& y, const float& width, const float& height);
	~Leaf();

	static void SetRenderMethod(ModelEntity *entity, ColorShaderClass *shader);

	void CreateRooms(ID3D11Device *device, ColorShaderClass* shader);

	void CreateHalls(ID3D11Device *device, Leaf *neighbour, ColorShaderClass* shader);

	bool Split();

	virtual void Update(const float& delta) override { }

	virtual void Render(D3DClass *direct, const D3DXMATRIX& projection, const D3DXMATRIX& view) override;

	inline bool IsSplit() const { return this->m_ChildA != nullptr && this->m_ChildB != nullptr; }

private:
	float m_X, m_Y, m_Width, m_Height;
	Leaf *m_ChildA, *m_ChildB;

	ModelEntity *m_Hall, *m_Back;
};