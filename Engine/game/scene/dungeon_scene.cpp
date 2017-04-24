#include "dungeon_scene.h"

#include <ctime>

#include "../../inputclass.h"
#include "../entity/dungeon/leaf.h"
#include "../../colorshaderclass.h"
#include "camera/camera_fixed.h"

#define SIZE 100

DungeonScene::DungeonScene(D3DClass* d3d, const HWND& hwnd, InputClass* input) 
: Scene(d3d, hwnd, input), m_RootLeaf(nullptr), m_ColorShader(nullptr)
{
	this->m_ColorShader = new ColorShaderClass;
	this->m_ColorShader->Initialize(d3d->GetDevice(), hwnd);

	CameraFixed *camera = new CameraFixed;
	camera->SetPointA(Vector3f(0.01f, 150.0f, 0.0f));
	camera->SetPointB(Vector3f(0.0f, 0.0f, 0.0f));
	Scene::SetCamera(camera);

	this->GenerateDungeon(d3d);
}

DungeonScene::~DungeonScene()
{
	if(this->m_RootLeaf != nullptr)
	{
		delete this->m_RootLeaf;
		this->m_RootLeaf = nullptr;
	}
	if(this->m_ColorShader != nullptr)
	{
		this->m_ColorShader->Shutdown();
		delete this->m_ColorShader;
		this->m_ColorShader = nullptr;
	}
}

void DungeonScene::GenerateDungeon(D3DClass* d3d)
{
	if(this->m_RootLeaf != nullptr)
	{
		delete this->m_RootLeaf;
	}
	// Seed random time
	srand(unsigned int(time(nullptr)));

	this->m_RootLeaf = new Leaf(nullptr, -SIZE / 2.0f, -SIZE / 2.0f, SIZE, SIZE);

	// Split our BSP
	bool split;
	do
	{
		split = this->m_RootLeaf->Split();
	} while (split);

	this->m_RootLeaf->CreateRooms(d3d->GetDevice(), this->m_ColorShader);
	this->m_RootLeaf->CreateHalls(d3d->GetDevice(), this->m_ColorShader);
}

bool DungeonScene::Update(const float& delta)
{
	if (Scene::m_Input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}
	return Scene::Update(delta);
}

void DungeonScene::Render(D3DClass* direct, const D3DXMATRIX& projection)
{
	const D3DXMATRIX& view = Scene::GetCamera()->GetViewMatrix();
	this->m_RootLeaf->Render(direct, projection, view);
}
