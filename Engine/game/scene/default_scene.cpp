#include "default_scene.h"

#include "../../inputclass.h"
#include "../entity/player.h"
#include "../world/world_grid.h"
#include "../../textureshaderclass.h"
#include "camera/camera_fixed.h"
#include "camera/camera_axis.h"

#define GRID_WORLD_SIZE 50.0f

#define CAMERA_SPEED 1.0f

DefaultScene::DefaultScene(D3DClass *d3d, const HWND& hwnd, InputClass *input) : Scene(d3d, hwnd, input)
{	
	this->m_TextureShader = new TextureShaderClass;
	this->m_TextureShader->Initialize(d3d->GetDevice(), hwnd);

	this->m_Player = new Player;

	this->m_WorldGrid = new WorldGrid(GRID_WORLD_SIZE);
	this->m_WorldGrid->SetShader(this->m_TextureShader);

	this->SetState(GameState::Map);
}

DefaultScene::~DefaultScene()
{
	if(this->m_Player != nullptr)
	{
		delete this->m_Player;
		this->m_Player = nullptr;
	}
	if(this->m_WorldGrid != nullptr)
	{
		delete this->m_WorldGrid;
		this->m_WorldGrid = nullptr;
	}
	if(this->m_TextureShader != nullptr)
	{
		this->m_TextureShader->Shutdown();
		delete this->m_TextureShader;
		this->m_TextureShader = nullptr;
	}
}

void DefaultScene::SetState(const GameState& state)
{
	this->m_State = state;
	if(state == GameState::Map)
	{
		CameraAxis *camera = new CameraAxis;
		camera->Translate(0.0f, 0.0f, -200.0f);
		//camera->SetPoints(Vector3f(0.0f, 0.0f, -200.0f), Vector3f(0.0f));
		Scene::SetCamera(camera);
	}
}

void DefaultScene::Update(const float& delta)
{
	Scene::Update(delta);

	this->m_Player->Update(delta);

	const D3DXVECTOR3& position = this->m_Player->GetPosition();
	this->m_WorldGrid->Update(Scene::m_Direct3D, position.x, position.z);

	InputClass *input = Scene::m_Input;
	if(this->m_State == GameState::Map)
	{
		Camera *camera = Scene::GetCamera();
		if (input->IsKeyDown(VK_W))
		{
			camera->Translate(0.0f, CAMERA_SPEED * delta, 0.0f);
		}
		if (input->IsKeyDown(VK_A))
		{
			camera->Translate(-CAMERA_SPEED * delta, 0.0f, 0.0f);
		}
		if (input->IsKeyDown(VK_S))
		{
			camera->Translate(0.0f, -CAMERA_SPEED * delta, 0.0f);
		}
		if(input->IsKeyDown(VK_D))
		{
			camera->Translate(CAMERA_SPEED * delta, 0.0f, 0.0f);
		}
	}
}

void DefaultScene::Render(D3DClass* direct, const D3DXMATRIX& projection)
{
	Camera *camera = Scene::GetCamera();
	if(camera == nullptr)
	{
		return;
	}
	const D3DXMATRIX& view = camera->GetViewMatrix();
	this->m_WorldGrid->Render(direct, projection, view);
}
