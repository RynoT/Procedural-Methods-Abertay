#include "island_model.h"

#include <math.h>
#include <limits>
#include <cassert>

#include "../../util/noise/perlinnoise.h"
#include "../../entity/island.h"

#define HEIGHT_INTENSITY 7.5f
#define RADIUS_ITERATIONS 16
#define ITERATION_OFFSET (1.0f / float(RADIUS_ITERATIONS))

#define PI 3.14159265358979f
#define PI2 (PI * 2.0f)

#define RES_DELTA (PI2 / 30.0f)
#define UV_MUL 8.0f

#define NOISE_FREQUENCY 4.0f
#define NOISE_INTENSITY 0.14f

#define FAULT_ITERATIONS 20
#define FAULT_DISPLACEMENT 0.01f

#define FLAT_TEXTURE_PATH L"data/textures/island_flat.png"
#define FAULT_TEXTURE_PATH L"data/textures/island_fault.png"
#define PERLIN_TEXTURE_PATH L"data/textures/island_perlin.png"

#define are_equal(a, b) (fabsf(a - b) < epsilon)
#define pseudo_random() (rand() / float(RAND_MAX))

IslandModel::IslandModel(IslandType *type) : m_Type(type), m_SurfaceLastIndex(0), m_SurfaceOuterLastIndex(0), m_BelowUpperLastIndex(0)
{
}

void IslandModel::AddNoise(std::vector<ModelData>& data) const
{
	switch (*this->m_Type)
	{
		case PERLIN:
			// Set the y to a random value using perlin noise
			for (int i = 0; i < this->m_SurfaceLastIndex; i++)
			{
				data[i].y = float(PerlinNoise::noise(data[i].x * NOISE_FREQUENCY, data[i].z * NOISE_FREQUENCY, 0.0f) * NOISE_INTENSITY);
			}
			break;
		case FAULT:
			// Iterate for a specific amount of times
			for (int i = 0; i < FAULT_ITERATIONS; i++)
			{
				// Create a random line accross the circle
				float v = pseudo_random() * PI / 2.0f;
				float a = sinf(v), b = cosf(v), c = pseudo_random() - 0.5f;
				for (int j = 0; j < this->m_SurfaceLastIndex; j++)
				{
					// If vertex is above line, increase y, else decrease
					ModelData& vertex = data[j];
					if (vertex.x * a + vertex.z * b - c > 0)
					{
						vertex.y += FAULT_DISPLACEMENT;
					}
					else
					{
						vertex.y -= FAULT_DISPLACEMENT;
					}
				}
			}
			break;

		case FLAT:
			// Flat terrain has no noise
		default: return;
	}
	// Move the hemisphere vertices to match up with the surface
	// TODO: If the surface goes below the top ring of the sphere then this will not work effectively.
	const float& epsilon = std::numeric_limits<float>::epsilon() * 100.0f;
	for (int i = this->m_SurfaceLastIndex; i < this->m_BelowUpperLastIndex; i++)
	{
		ModelData& bottom = data[i];
		if (bottom.y != 0.0f)
		{
			continue;
		}
		for (int j = 0; j < this->m_SurfaceOuterLastIndex; j++)
		{
			ModelData& surface = data[j];
			if (!are_equal(surface.x, bottom.x) || !are_equal(surface.z, bottom.z))
			{
				continue;
			}
			bottom.y = surface.y;
			break;
		}
	}
}

bool IslandModel::Initialize(ID3D11Device *device)
{
	ModelData v1, v2, v3, v4;
	std::vector<ModelData> data;

	// Generate the surface (disk)
	for (int itr = RADIUS_ITERATIONS; itr >= 0; itr--)
	{
		float r1 = (itr * ITERATION_OFFSET), r2 = ((itr - 1) * ITERATION_OFFSET);
		for (float theta = RES_DELTA; theta < PI2; theta += RES_DELTA)
		{
			v1.x = std::cosf(theta) * r1;
			v1.y = 0.0f;
			v1.z = std::sinf(theta) * r1;
			v1.tu = 0.5f + v1.x / 2.0f * UV_MUL;
			v1.tv = 0.5f + v1.z / 2.0f * UV_MUL;
			v1.nx = v1.nz = 0.0f;
			v1.ny = 1.0f;

			v2.x = std::cosf(theta) * r2;
			v2.y = 0.0f;
			v2.z = std::sinf(theta) * r2;
			v2.tu = 0.5f + v2.x / 2.0f * UV_MUL;
			v2.tv = 0.5f + v2.z / 2.0f * UV_MUL;
			v2.nx = v2.nz = 0.0f;
			v2.ny = 1.0f;

			v3.x = std::cosf(theta - RES_DELTA) * r1;
			v3.y = 0.0f;
			v3.z = std::sinf(theta - RES_DELTA) * r1;
			v3.tu = 0.5f + v3.x / 2.0f * UV_MUL;
			v3.tv = 0.5f + v3.z / 2.0f * UV_MUL;
			v3.nx = v3.nz = 0.0f;
			v3.ny = 1.0f;

			v4.x = std::cosf(theta - RES_DELTA) * r2;
			v4.y = 0.0f;
			v4.z = std::sinf(theta - RES_DELTA) * r2;
			v4.tu = 0.5f + v4.x / 2.0f * UV_MUL;
			v4.tv = 0.5f + v4.z / 2.0f * UV_MUL;
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
		if (itr == RADIUS_ITERATIONS)
		{
			this->m_SurfaceOuterLastIndex = data.size();
		}
	}
	this->m_SurfaceLastIndex = data.size();

	// Generate the underneith (hemisphere)
	const float theta = PI2 / 30.0f, delta = PI / 30.0f;
	//for (float latitude = 0.0f; latitude < PI / 2.0f; latitude += delta)
	for (float latitude = PI / 2.0f - delta; latitude >= 0.0f; latitude -= delta)
	{
		for (float longitude = PI2; longitude >= 0.0f; longitude -= theta)
		{
			v1.x = v1.nx = std::cos(longitude + theta) * std::sin(latitude + delta);
			v1.y = v1.ny = std::cos(latitude + delta);
			v1.z = v1.nz = std::sin(longitude + theta) * std::sin(latitude + delta);
			v1.tu = 1.0f - (longitude + theta) / PI2 - 0.25f;
			v1.tv = (latitude + delta) / PI;

			v2.x = v1.nx = std::cos(longitude) * std::sin(latitude + delta);
			v2.y = v1.ny = std::cos(latitude + delta);
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
				v1.y = v2.y = 0.0f;

				// TODO make this more efficient?
				//v1.y = float(PerlinNoise::noise(v1.x * NOISE_FREQUENCY, v1.z * NOISE_FREQUENCY, 0.0f) * NOISE_INTENSITY);
				//v2.y = float(PerlinNoise::noise(v2.x * NOISE_FREQUENCY, v2.z * NOISE_FREQUENCY, 0.0f) * NOISE_INTENSITY);
			}

			// Push vertices as triangles
			data.push_back(v1);
			data.push_back(v2);
			data.push_back(v3);

			data.push_back(v3);
			data.push_back(v4);
			data.push_back(v1);
		}
		if (latitude >= PI / 2.0f - delta)
		{
			this->m_BelowUpperLastIndex = data.size();
		}
	}
	this->AddNoise(data);

	ModelMesh *mesh = Model::CreateMesh();
	ModelData *modelData = mesh->SetVertexCount(data.size());
	// Set mesh and apply perlin noise to surface
	for (size_t i = 0; i < data.size(); i++)
	{
		modelData[i] = data[i];
	}
	// Give each type a different texture
	WCHAR *texturePath;
	switch (*this->m_Type)
	{
		case PERLIN:
			texturePath = PERLIN_TEXTURE_PATH;
			break;
		case FAULT:
			texturePath = FAULT_TEXTURE_PATH;
			break;
		case FLAT:
			texturePath = FLAT_TEXTURE_PATH;
			break;
		default: assert(false); return false;
	}
	return Model::LoadTexture(device, texturePath) && Model::InitializeVerticesTexture(device) && Model::InitializeIndices(device);
}
