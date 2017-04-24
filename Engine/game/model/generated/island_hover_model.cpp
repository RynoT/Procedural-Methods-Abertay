#include "island_hover_model.h"

#define SIZE 1.0f
#define INDENT 0.2f
#define Y_OFFSET -0.0f

#define SURFACE_TEXTURE_PATH L"data/textures/island_hover.dds"

bool IslandHoverModel::Initialize(ID3D11Device* device)
{
	int indices[] = { 0, 1, 2, 2, 3, 0 };
	float vertex[] =
	{
		-SIZE, Y_OFFSET, SIZE, //top
		-SIZE + INDENT, Y_OFFSET, SIZE - INDENT,
		SIZE - INDENT, Y_OFFSET, SIZE - INDENT,
		SIZE, Y_OFFSET, SIZE,

		SIZE, Y_OFFSET, SIZE, //right
		SIZE - INDENT, Y_OFFSET, SIZE - INDENT,
		SIZE - INDENT, Y_OFFSET, -SIZE + INDENT,
		SIZE, Y_OFFSET, -SIZE,

		-SIZE, Y_OFFSET, -SIZE, //bottom
		SIZE, Y_OFFSET, -SIZE,
		SIZE - INDENT, Y_OFFSET, -SIZE + INDENT,
		-SIZE + INDENT, Y_OFFSET, -SIZE + INDENT,

		-SIZE, Y_OFFSET, SIZE, //left
		-SIZE, Y_OFFSET, -SIZE,
		-SIZE + INDENT, Y_OFFSET, -SIZE + INDENT,
		-SIZE + INDENT, Y_OFFSET, SIZE - INDENT
	};

	ModelMesh *mesh = Model::CreateMesh();
	ModelData *data = mesh->SetVertexCount(4*6);
	for(int i = 0; i < 4*6; i++)
	{
		int idx = (indices[i % 6] + 4 * (i / 6)) * 3;
		data[i].x = vertex[idx];
		data[i].y = vertex[idx + 1];
		data[i].z = vertex[idx + 2];

		data[i].tu = data[i].tv = 0.0f;
		data[i].nx = data[i].ny = 0.0f;
		data[i].nz = 1.0f;
	}
	return Model::LoadTexture(device, SURFACE_TEXTURE_PATH) && Model::InitializeVerticesTexture(device) && Model::InitializeIndices(device);
}
