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

float Island::GetSurfaceY(float x, float z) const
{
	x -= Entity::GetPosition().x;
	z -= Entity::GetPosition().z;

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
		return v1.y * w1 + v2.y * w2 + v3.y * w3;
	}
	return 0.0f;
}

//function pointInTriangle(x1, y1, x2, y2, x3, y3, x, y:Number) :Boolean
//{
//	var denominator : Number = (x1*(y2 - y3) + y1*(x3 - x2) + x2*y3 - y2*x3);
//var t1 : Number = (x*(y3 - y1) + y*(x1 - x3) - x1*y3 + y1*x3) / denominator;
//var t2 : Number = (x*(y2 - y1) + y*(x1 - x2) - x1*y2 + y1*x2) / -denominator;
//var s : Number = t1 + t2;
//
//return 0 <= t1 && t1 <= 1 && 0 <= t2 && t2 <= 1 && s <= 1;
//}