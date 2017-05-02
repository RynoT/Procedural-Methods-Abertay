#include "aabb.h"

#include <limits>
#include <algorithm>

#include "../model_mesh.h"

#undef min
#undef max

CollisionAABB::CollisionAABB() : m_Max(0.0f), m_Min(0.0f)
{
}

void CollisionAABB::Set(ModelMesh* mesh)
{
	// Reset min and max to their opposite extremes
	this->m_Min.set(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
	this->m_Max.set(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min());

	// Set min a max to the extremes of the mesh vertex data. This will create an AABB
	const ModelData *data = mesh->GetModelData();
	for(int i = 0; i < mesh->GetVertexCount(); i++)
	{
		this->m_Min.x = std::min(this->m_Min.x, data[i].x);
		this->m_Min.y = std::min(this->m_Min.y, data[i].y);
		this->m_Min.z = std::min(this->m_Min.z, data[i].z);

		this->m_Max.x = std::max(this->m_Max.x, data[i].x);
		this->m_Max.y = std::max(this->m_Max.y, data[i].y);
		this->m_Max.z = std::max(this->m_Max.z, data[i].z);
	}
}

bool CollisionAABB::Intersects(const Vector3f& point)
{
	return point.x >= this->m_Min.x && point.x <= this->m_Max.x
		&& point.y >= this->m_Min.y && point.y <= this->m_Max.y
		&& point.z >= this->m_Min.z && point.z <= this->m_Max.z;
}
