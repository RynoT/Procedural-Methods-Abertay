#include "model_mesh.h"

#include <assert.h>

#include "collision/collision.h"

ModelMesh::ModelMesh(const int& count) : m_Count(count), m_Collision(nullptr)
{
	assert(count >= 0 && "Count must be >= 0");
	this->m_ModelData = new ModelData[count];
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
