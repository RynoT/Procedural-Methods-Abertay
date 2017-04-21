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

#define NOISE_FREQUENCY 4.0f
#define NOISE_INTENSITY 0.18f

#define SURFACE_TEXTURE_PATH L"data/textures/seafloor.dds"

IslandModel::IslandModel() : m_SurfaceLastIndex(0)
{
}

bool IslandModel::Initialize(ID3D11Device *device)
{
	ModelData v1, v2, v3, v4;
	std::vector<ModelData> data;

	// Generate the surface (disk)
	for(float theta = RES_DELTA; theta < PI2; theta += RES_DELTA)
	{
		for(int itr = 1; itr <= RADIUS_ITERATIONS; itr++)
		{
			float rt = ITERATION_OFFSET * RADIUS_ITERATIONS;
			float r1 = (itr * ITERATION_OFFSET), r2 = ((itr - 1) * ITERATION_OFFSET);
			v1.x = std::cosf(theta) * r1;
			v1.y = 0.0f;
			v1.z = std::sinf(theta) * r1;
			v1.tu = (v1.x/r1) / (rt * 2.0f) + 0.5f;
			v1.tv = (v1.y/r1) / (rt * 2.0f) + 0.5f;
			v1.nx = v1.nz = 0.0f;
			v1.ny = 1.0f;

			v2.x = std::cosf(theta) * r2;
			v2.y = 0.0f;
			v2.z = std::sinf(theta) * r2;
			v2.tu = (v2.x/r2) / (rt * 2.0f) + 0.5f;
			v2.tv = (v2.y/r2) / (rt * 2.0f) + 0.5f;
			v2.nx = v2.nz = 0.0f;
			v2.ny = 1.0f;

			v3.x = std::cosf(theta - RES_DELTA) * r1;
			v3.y = 0.0f;
			v3.z = std::sinf(theta - RES_DELTA) * r1;
			v3.tu = (v3.x/r1) / (rt * 2.0f) + 0.5f;
			v3.tv = (v3.y/r1) / (rt * 2.0f) + 0.5f;
			v3.nx = v3.nz = 0.0f;
			v3.ny = 1.0f;

			v4.x = std::cosf(theta - RES_DELTA) * r2;
			v4.y = 0.0f;
			v4.z = std::sinf(theta - RES_DELTA) * r2;
			v4.tu = (v4.x/r2) / (rt * 2.0f) + 0.5f;
			v4.tv = (v4.y/r2) / (rt * 2.0f) + 0.5f;
			v4.nx = v4.nz = 0.0f;
			v4.ny = 1.0f;

			// Push vertices as triangles
			data.push_back(v4);
			data.push_back(v1);
			data.push_back(v2);

			data.push_back(v3);
			data.push_back(v1);
			data.push_back(v4);
		}
	}
	this->m_SurfaceLastIndex = data.size();
	for(int i = 0; i < this->m_SurfaceLastIndex; i++)
	{
		data[i].y = PerlinNoise::noise(data[i].x * NOISE_FREQUENCY, data[i].z * NOISE_FREQUENCY, 0.0f) * NOISE_INTENSITY;
	}

	// Generate the underneith (hemisphere)
	const float theta = PI2 / 30.0f, delta = PI / 30.0f;
	for (float longitude = 0.0f; longitude < PI2; longitude += theta)
	{
		for (float latitude = 0; latitude < PI / 2.0f; latitude += delta)
		{
			v1.x = v1.nx = std::cos(longitude + theta) * std::sin(latitude + delta);
			v1.y = v1.ny = std::cos(latitude + delta);
			v1.z = v1.nz = std::sin(longitude + theta) * std::sin(latitude + delta);
			v1.tu = 1.0f - (longitude + theta) / PI2 - 0.25f;
			v1.tv = (latitude + delta) / PI;

			v2.x = v1.nx = std::cos(longitude) * std::sin(latitude + delta);
			v2.y = v1.ny =  std::cos(latitude + delta);
			v2.z = v1.nz = std::sin(longitude) * std::sin(latitude + delta);
			v2.tu = 1.0f - longitude / PI2 - 0.25f;
			v2.tv = (latitude + delta - PI / 180.0f) / PI;

			v3.x = v1.nx = std::cos(longitude) * std::sin(latitude);
			v3.y = v1.ny = std::cos(latitude);
			v3.z = v1.nz = std::sin(longitude) * std::sin(latitude);
			v3.tu = 1.0f - longitude / PI2 - 0.25f;
			v3.tv = latitude / PI;

			v4.x = v1.nx = std::cos(longitude + theta) * std::sin(latitude);
			v4.y = v1.ny = std::cos(latitude);
			v4.z = v1.nz = std::sin(longitude + theta) * std::sin(latitude);
			v4.tu = 1.0f - (longitude + theta) / PI2 - 0.25f;
			v4.tv = latitude / PI;

			if (latitude >= PI / 2.0f - delta)
			{
				// TODO make this more efficient?
				v1.y = PerlinNoise::noise(v1.x * NOISE_FREQUENCY, v1.z * NOISE_FREQUENCY, 0.0f) * NOISE_INTENSITY;
				v2.y = PerlinNoise::noise(v2.x * NOISE_FREQUENCY, v2.z * NOISE_FREQUENCY, 0.0f) * NOISE_INTENSITY;
			}

			// Push vertices as triangles
			data.push_back(v1);
			data.push_back(v2);
			data.push_back(v3);

			data.push_back(v3);
			data.push_back(v4);
			data.push_back(v1);
		}
	}

	ModelMesh *mesh = Model::CreateMesh();
	ModelData *modelData = mesh->SetVertexCount(data.size());
	// Set mesh and apply perlin noise to surface
	for(int i = 0; i < data.size(); i++)
	{
		modelData[i] = data[i];
	}
	return Model::LoadTexture(device, SURFACE_TEXTURE_PATH) && Model::Initialize(device);
}
