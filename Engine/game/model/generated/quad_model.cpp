#include "quad_model.h"

#define SIZE 1.0f
#define HSIZE (SIZE / 2.0f)

QuadModel::QuadModel(const float& r, const float& g, const float& b, const float& a) : m_R(r), m_G(g), m_B(b), m_A(a)
{
}

bool QuadModel::Initialize(ID3D11Device* device)
{
	int indices[] = { 0, 1, 2, 2, 3, 0 };
	float vertices[] =
	{
		-HSIZE, 0.0f, -HSIZE, this->m_R, this->m_G, this->m_B, this->m_A,
		-HSIZE, 0.0f, HSIZE, this->m_R, this->m_G, this->m_B, this->m_A,
		HSIZE, 0.0f, HSIZE, this->m_R, this->m_G, this->m_B, this->m_A,
		HSIZE, 0.0f, -HSIZE, this->m_R, this->m_G, this->m_B, this->m_A,
	};

	ModelMesh *mesh = Model::CreateMesh();
	ModelData *modelData = mesh->SetVertexCount(6);
	for(int i = 0; i < 6; i++)
	{
		int idx = indices[i] * 7;
		modelData[i].x = vertices[idx];
		modelData[i].y = vertices[idx + 1];
		modelData[i].z = vertices[idx + 2];

		modelData[i].cr = vertices[idx + 3];
		modelData[i].cg = vertices[idx + 4];
		modelData[i].cb = vertices[idx + 5];
		modelData[i].ca = vertices[idx + 6];
	}

	return Model::InitializeVerticesColor(device) && Model::InitializeIndices(device);
}
