#include "island_hover_model.h"

#define SIZE 1.0f
#define INDENT 0.2f
#define Z_OFFSET -0.0f

#define SURFACE_TEXTURE_PATH L"data/textures/island_hover.dds"

bool IslandHoverModel::Initialize(ID3D11Device* device)
{
	int indices[] = { 0, 2, 1, 2, 0, 3 };
	float vertex[] =
	{
		-SIZE, SIZE, Z_OFFSET, //top
		-SIZE + INDENT, SIZE - INDENT, Z_OFFSET,
		SIZE - INDENT, SIZE - INDENT, Z_OFFSET,
		SIZE, SIZE, Z_OFFSET,

		SIZE, SIZE, Z_OFFSET, //right
		SIZE - INDENT, SIZE - INDENT, Z_OFFSET,
		SIZE - INDENT, -SIZE + INDENT, Z_OFFSET,
		SIZE, -SIZE, Z_OFFSET,

		-SIZE, -SIZE, Z_OFFSET, //bottom
		SIZE, -SIZE, Z_OFFSET,
		SIZE - INDENT, -SIZE + INDENT, Z_OFFSET,
		-SIZE + INDENT, -SIZE + INDENT, Z_OFFSET,

		-SIZE, SIZE, Z_OFFSET, //left
		-SIZE, -SIZE, Z_OFFSET,
		-SIZE + INDENT, -SIZE + INDENT, Z_OFFSET,
		-SIZE + INDENT, SIZE - INDENT, Z_OFFSET,
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
	return Model::LoadTexture(device, SURFACE_TEXTURE_PATH) && Model::Initialize(device);
}
