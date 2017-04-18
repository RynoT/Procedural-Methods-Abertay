#include "model_mesh.h"

#include <assert.h>

#include "collision/collision.h"

ModelMesh::ModelMesh() : m_Count(0), m_Collision(nullptr), m_ModelData(nullptr)
{
}

ModelMesh::~ModelMesh()
{
	if(this->m_Collision != nullptr)
	{
		delete this->m_Collision;
		this->m_Collision = nullptr;
	}
	if(this->m_ModelData != nullptr)
	{
		delete[] this->m_ModelData;
		this->m_ModelData = nullptr;
	}
}

ModelData* ModelMesh::SetVertexCount(const int& count)
{
	this->m_Count = count;
	if(this->m_ModelData != nullptr)
	{
		delete[] this->m_ModelData;
	}
	assert(count >= 0 && "Count must be >= 0");
	return this->m_ModelData = new ModelData[count];
}

void ModelMesh::SetCollision(Collision *collision)
{
	if(this->m_Collision != nullptr)
	{
		delete this->m_Collision;
	}
	this->m_Collision = collision;
	if(collision != nullptr)
	{
		collision->Set(this);
	}
}
