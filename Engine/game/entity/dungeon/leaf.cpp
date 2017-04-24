#include "leaf.h"

#include <math.h>

#include "../../../colorshaderclass.h"
#include "../../model/generated/quad_model.h"

#define MIN_SIZE 18
#define MIN_ROOM_SIZE 6
#define ROOM_GAP 3
#define HALL_WIDTH 2
#define SPLIT_OVERRIDE_PERC 1.25f

#define pseudo_random() (rand() / float(RAND_MAX)) // Random number between 0-1
#define random(min, max) ((min) + ((max) - (min)) * pseudo_random())
#define center_pos(e, xp, yp) xp + e->GetScale().x / 2.0f, 0.0f, yp + e->GetScale().z / 2.0f
#define leaf_distance(a, from) (from - a->GetPosition()).GetLengthSq()

Leaf::Leaf(Leaf *parent, const float& x, const float& y, const float& width, const float& height)
	: m_X(x), m_Y(y), m_Width(width), m_Height(height), m_Parent(parent),
	m_ChildA(nullptr), m_ChildB(nullptr), m_Hall(nullptr), m_Back(nullptr)
{
	Entity::SetPosition(x + width / 2, 0.0f, y + height / 2);
}

Leaf::~Leaf()
{
	if (this->m_Hall != nullptr)
	{
		delete this->m_Hall;
		this->m_Hall = nullptr;
	}
	if (this->m_Back != nullptr)
	{
		delete this->m_Back;
		this->m_Back = nullptr;
	}
	if (this->m_ChildA != nullptr)
	{
		delete this->m_ChildA;
		this->m_ChildA = nullptr;
	}
	if (this->m_ChildB != nullptr)
	{
		delete this->m_ChildB;
		this->m_ChildB = nullptr;
	}
}

void Leaf::SetRenderMethod(ModelEntity *entity, ColorShaderClass* shader)
{
	entity->SetRenderMethod([entity, shader](D3DClass* direct, const D3DXMATRIX& projection,
		const D3DXMATRIX& view, const D3DXMATRIX& model)->void
	{
		Model *imodel = entity->GetInternalModel();
		shader->Render(direct->GetDeviceContext(), imodel->GetIndexCount(), model, view, projection);
	});
}

Leaf* Leaf::GetClosestTo(const Vector3f& position)
{
	if(this->IsSplit())
	{
		Leaf *l1 = this->m_ChildA->GetClosestTo(position), *l2 = this->m_ChildB->GetClosestTo(position);
		return leaf_distance(l1, position) < leaf_distance(l2, position) ? l1 : l2;
	}
	return this;
}

void Leaf::CreateHalls(ID3D11Device* device, ColorShaderClass* shader)
{
	if(this->IsSplit())
	{
		this->m_ChildA->CreateHalls(device, shader);
		this->m_ChildB->CreateHalls(device, shader);

		Leaf *neighbourA = this->m_ChildA->GetClosestTo(this->m_ChildB->GetPosition());
		Leaf *neighbourB = this->m_ChildB->GetClosestTo(this->m_ChildA->GetPosition());
		this->CreateHall(device, neighbourA->GetPosition(), neighbourB->GetPosition(), shader);
	}
}

void Leaf::CreateHall(ID3D11Device* device, const Vector3f& pointA, const Vector3f& pointB, ColorShaderClass* shader)
{
	this->m_Hall = new ModelEntity;
	this->m_Hall->SetModel(new QuadModel(1.0f, 0.0, 0.0f));
	this->m_Hall->GetInternalModel()->Initialize(device);
	Leaf::SetRenderMethod(this->m_Hall, shader);

	Vector3f vector = pointB - pointA;
	float length = vector.GetLength();

	float theta = std::atan2f(pointB.x - pointA.x, pointB.z - pointA.z);
	this->m_Hall->SetPosition(VECTOR3_SPLIT(pointA + vector.normalize().multiply(length / 2.0f) + Vector3f(0.0f, -0.02f, 0.0f)));
	this->m_Hall->SetScale(HALL_WIDTH, 0.0f, length);
	this->m_Hall->SetRotation(0.0f, theta, 0.0f);
}

void Leaf::CreateRooms(ID3D11Device *device, ColorShaderClass* shader)
{
	if (this->IsSplit())
	{
		this->m_ChildA->CreateRooms(device, shader);
		this->m_ChildB->CreateRooms(device, shader);
	}
	else
	{
		// Setup room
		QuadModel *room = new QuadModel(pseudo_random(), pseudo_random(), pseudo_random());
		ModelEntity::SetModel(room);
		ModelEntity::GetInternalModel()->Initialize(device);
		Leaf::SetRenderMethod(this, shader);

		float width = MIN_ROOM_SIZE + (this->m_Width - ROOM_GAP * 2.0f - MIN_ROOM_SIZE) * pseudo_random();
		float height = MIN_ROOM_SIZE + (this->m_Height - ROOM_GAP * 2.0f - MIN_ROOM_SIZE) * pseudo_random();
		float x = ROOM_GAP + (this->m_Width - width - ROOM_GAP * 2.0f) * pseudo_random();
		float y = ROOM_GAP + (this->m_Height - height - ROOM_GAP * 2.0f) * pseudo_random();

		ModelEntity::SetPosition(this->m_X + x + width / 2.0f, 0.0f, this->m_Y + y + height / 2.0f);
		ModelEntity::SetScale(width, 0.0f, height);

		// Setup back
		this->m_Back = new ModelEntity;
		this->m_Back->SetModel(new QuadModel(1.0f - room->GetRed(), 1.0f - room->GetGreen(), 1.0f - room->GetBlue()));
		this->m_Back->GetInternalModel()->Initialize(device);
		Leaf::SetRenderMethod(this->m_Back, shader);

		this->m_Back->SetPosition(this->m_X + this->m_Width / 2.0f, -0.05f, this->m_Y + this->m_Height / 2.0f);
		this->m_Back->SetScale(this->m_Width, 0.0f, this->m_Height);
	}
}

bool Leaf::Split()
{
	if (this->IsSplit())
	{
		return this->m_ChildA->Split() || this->m_ChildB->Split();
	}
	// Decide whether we split vertical or horizontal 
	bool horizontal = pseudo_random() > 0.5f;
	if (horizontal && this->m_Width > this->m_Height
		&& this->m_Width / this->m_Height >= SPLIT_OVERRIDE_PERC)
	{
		horizontal = false;
	}
	else if (!horizontal && this->m_Height > this->m_Width
		&& this->m_Height / this->m_Width >= SPLIT_OVERRIDE_PERC)
	{
		horizontal = true;
	}

	// Get our child leafs maximum size
	float max = (horizontal ? this->m_Height : this->m_Width) - MIN_SIZE;
	if (max <= MIN_SIZE)
	{
		// Dont split if our current size is too small
		return false;
	}

	float split = MIN_SIZE + (max - MIN_SIZE) * pseudo_random();
	if (horizontal)
	{
		this->m_ChildA = new Leaf(this, this->m_X, this->m_Y, this->m_Width, split);
		this->m_ChildB = new Leaf(this, this->m_X, this->m_Y + split, this->m_Width, this->m_Height - split);
	}
	else
	{
		this->m_ChildA = new Leaf(this, this->m_X, this->m_Y, split, this->m_Height);
		this->m_ChildB = new Leaf(this, this->m_X + split, this->m_Y, this->m_Width - split, this->m_Height);
	}
	return true;
}

void Leaf::Render(D3DClass* direct, const D3DXMATRIX& projection, const D3DXMATRIX& view)
{
	if (this->m_Hall != nullptr)
	{
		this->m_Hall->Render(direct, projection, view);
	}
	if (this->IsSplit())
	{
		this->m_ChildA->Render(direct, projection, view);
		this->m_ChildB->Render(direct, projection, view);
	}
	else
	{
		if (this->m_Back != nullptr)
		{
			this->m_Back->Render(direct, projection, view);
		}
		ModelEntity::Render(direct, projection, view);
	}
}
