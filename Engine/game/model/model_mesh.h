#pragma once

class Collision;

struct ModelData
{
	float x, y, z;
	float tu, tv;
	float nx, ny, nz;
};

class ModelMesh
{
public:
	ModelMesh();
	~ModelMesh();

	ModelData* SetVertexCount(const int& count);

	void SetCollision(Collision *collision);

	inline const int& GetVertexCount() const { return this->m_Count; }

	inline const Collision* GetCollision() const { return this->m_Collision; }

	inline const ModelData* GetModelData() const { return this->m_ModelData; }

private:
	int m_Count;

	Collision *m_Collision;
	ModelData *m_ModelData;
};