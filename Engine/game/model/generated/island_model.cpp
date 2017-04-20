#include "island_model.h"

#include <math.h>
#include <vector>

#include "../../util/noise/perlinnoise.h"

#define HEIGHT_INTENSITY 10.0f
#define RADIUS_ITERATIONS 16
#define ITERATION_OFFSET (1.0f / float(RADIUS_ITERATIONS))

#define PI 3.14159265358979f
#define PI2 (PI * 2.0f)

#define RES_DELTA (PI2 / 30.0f)

#define SURFACE_TEXTURE_PATH L"data/textures/seafloor.dds"

IslandModel::IslandModel(const float& x, const float& y) : m_X(x), m_Y(y)
{
}

bool IslandModel::Initialize(ID3D11Device *device)
{
	std::vector<ModelData> data;

	// Generate the surface (disk)
	for(float theta = RES_DELTA; theta < PI2; theta += RES_DELTA)
	{
		for(int itr = 1; itr < RADIUS_ITERATIONS; itr++)
		{
			ModelData v1, v2, v3, v4;
			v1.x = std::cosf(theta) * (itr * ITERATION_OFFSET);
			v1.y = 0.0f;
			v1.z = std::sinf(theta) * (itr * ITERATION_OFFSET);
			v1.tu = v1.tv = 0.0f;
			v1.nx = v1.ny = 0.0f;
			v1.nz = 1.0f;

			v2.x = std::cosf(theta) * ((itr - 1) * ITERATION_OFFSET);
			v2.y = 0.0f;
			v2.z = std::sinf(theta) * ((itr - 1) * ITERATION_OFFSET);
			v2.tu = v2.tv = 0.0f;
			v2.nx = v2.ny = 0.0f;
			v2.nz = 1.0f;

			v3.x = std::cosf(theta - RES_DELTA) * (itr * ITERATION_OFFSET);
			v3.y = 0.0f;
			v3.z = std::sinf(theta - RES_DELTA) * (itr * ITERATION_OFFSET);
			v3.tu = v3.tv = 0.0f;
			v3.nx = v3.ny = 0.0f;
			v3.nz = 1.0f;

			v4.x = std::cosf(theta - RES_DELTA) * ((itr - 1) * ITERATION_OFFSET);
			v4.y = 0.0f;
			v4.z = std::sinf(theta - RES_DELTA) * ((itr - 1) * ITERATION_OFFSET);
			v4.tu = v4.tv = 0.0f;
			v4.nx = v4.ny = 0.0f;
			v4.nz = 1.0f;

			// Push vertices as triangles

			data.push_back(v4);
			data.push_back(v1);
			data.push_back(v2);

			data.push_back(v3);
			data.push_back(v1);
			data.push_back(v4);
		}
	}
	ModelMesh *mesh = Model::CreateMesh();
	ModelData *modelData = mesh->SetVertexCount(data.size());
	// Set mesh and apply perlin noise to surface
	for(int i = 0; i < data.size(); i++)
	{
		//data[i].z = PerlinNoise::noise(this->m_X, this->m_Y, (this->m_X + this->m_Y) / 2.0f);
		modelData[i] = data[i];
	}
	return Model::LoadTexture(device, SURFACE_TEXTURE_PATH) && Model::Initialize(device);
}
