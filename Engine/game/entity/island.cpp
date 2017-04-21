#include "island.h"
#include "../model/generated/island_model.h"
#include "../model/collision/aabb.h"
#include "../../systemclass.h"

Island::Island(ID3D11Device* device)
{
	Model *model = new IslandModel;
	model->Initialize(device);
	model->SetCollision(new CollisionAABB);

	ModelEntity::SetModel(model);
}

bool Island::GetSurfaceY(float x, float z, float& out) const
{
	x -= Entity::GetPosition().x;
	z -= Entity::GetPosition().z;
	if(x == 0.0f && z == 0.0f)
	{
		// Stop dividing by 0 error by shifting the coordinates by a very small distance
		x = z = 0.00001f;
	}
	Vector3f v1, v2, v3;
	IslandModel *model = (IslandModel*)ModelEntity::GetInternalModel();
	const ModelData *data = model->GetModelMesh()->GetModelData();
	for (int i = 0; i < model->GetLastSurfaceIndex() - 3;)
	{
		v1.set(data[i].x, data[i].y, data[i].z) *= Entity::GetScale(); i++;
		v2.set(data[i].x, data[i].y, data[i].z) *= Entity::GetScale(); i++;
		v3.set(data[i].x, data[i].y, data[i].z) *= Entity::GetScale(); i++;

		// Check to see if point is inside triangle using barycentric coordinates
		float w1 = ((v2.z - v3.z) * (x - v3.x) + (v3.x - v2.x) * (z - v3.z)) / ((v2.z - v3.z) * (v1.x - v3.x) + (v3.x - v2.x) * (v1.z - v3.z));
		if (w1 < 0.0f || w1 > 1.0f) {
			continue;
		}
		float w2 = ((v3.z - v1.z) * (x - v3.x) + (v1.x - v3.x) * (z - v3.z)) / ((v2.z - v3.z) * (v1.x - v3.x) + (v3.x - v2.x) * (v1.z - v3.z));
		if (w2 < 0.0f || w2 > 1.0f) {
			continue;
		}
		float w3 = 1.0f - w1 - w2;
		if (w3 < 0.0f || w3 > 1.0f) {
			continue;
		}
		out = v1.y * w1 + v2.y * w2 + v3.y * w3;
		return true;
	}
	return false;
}