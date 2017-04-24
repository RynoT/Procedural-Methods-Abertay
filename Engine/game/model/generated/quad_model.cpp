#include "quad_model.h"

bool QuadModel::Initialize(ID3D11Device* device)
{
	ModelMesh *mesh = Model::CreateMesh();
	ModelData *modelData = mesh->SetVertexCount(6);

	return Model::InitializeVerticesTexture(device) && Model::InitializeIndices(device);
}
