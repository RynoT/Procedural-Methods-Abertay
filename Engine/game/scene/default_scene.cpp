#include "default_scene.h"

#include <cassert>

#include "../world/world_grid.h"
#include "../entity/player.h"
#include "../../inputclass.h"
#include "../../textureshaderclass.h"
#include "../../systemclass.h"
#include "camera/camera_fpv.h"
#include "camera/camera_transition.h"
#include "../entity/island.h"
#include "../model/generated/island_hover_model.h"

#define GRID_SIZE 9
#define GRID_WORLD_SIZE 2000.0f

#define CAMERA_SPEED 2000.0f
#define CAMERA_SURFACE_SPEED 36.0f

#define MAP_CAMERA_Y (-GRID_WORLD_SIZE * 4.0f)

#define MAP_ZOOM_TOTAL 7500.0f
#define MAP_MIN_ZOOM (MAP_CAMERA_Y - MAP_ZOOM_TOTAL / 2.0f)
#define MAP_MAX_ZOOM (MAP_CAMERA_Y + MAP_ZOOM_TOTAL / 2.0f)

DefaultScene::DefaultScene(D3DClass *d3d, const HWND& hwnd, InputClass *input) 
: Scene(d3d, hwnd, input), m_bTransitioning(false), m_HoveredCell(nullptr)
{
	this->m_TextureShader = new TextureShaderClass;
	this->m_TextureShader->Initialize(d3d->GetDevice(), hwnd);

	this->m_Player = new Player;

	this->m_IslandHover = new ModelEntity;
	this->m_IslandHover->SetModel(new IslandHoverModel);
	this->m_IslandHover->GetInternalModel()->Initialize(d3d->GetDevice());
	this->m_IslandHover->SetRenderMethod([this](D3DClass* direct, const D3DXMATRIX& projection,
		const D3DXMATRIX& view, const D3DXMATRIX& model)->void
	{
		auto imodel = this->m_IslandHover->GetInternalModel();
		this->m_TextureShader->Render(direct->GetDeviceContext(), imodel->GetIndexCount(), model, view, projection, imodel->GetTexture());
	});

	this->m_WorldGrid = new WorldGrid(GRID_WORLD_SIZE, GRID_SIZE);
	this->m_WorldGrid->SetShader(this->m_TextureShader);

	D3DXMatrixIdentity(&this->m_LastView);
	D3DXMatrixIdentity(&this->m_LastProjection);

	this->SetState(GameState::Map);
	Scene::SetCamera(new CameraTransition(Vector3f(0.0f, MAP_CAMERA_Y, 0.0f), Vector3f(90.0f, 0.0f, 0.0f)));
}

DefaultScene::~DefaultScene()
{
	if (this->m_Player != nullptr)
	{
		delete this->m_Player;
		this->m_Player = nullptr;
	}
	if (this->m_IslandHover != nullptr)
	{
		delete this->m_IslandHover;
		this->m_IslandHover = nullptr;
	}
	if (this->m_WorldGrid != nullptr)
	{
		delete this->m_WorldGrid;
		this->m_WorldGrid = nullptr;
	}
	if (this->m_TextureShader != nullptr)
	{
		this->m_TextureShader->Shutdown();
		delete this->m_TextureShader;
		this->m_TextureShader = nullptr;
	}
	this->m_HoveredCell = nullptr;
}

void DefaultScene::SetState(const GameState& state)
{
	this->m_State = state;
	SystemClass::SetMouseGrab(state != GameState::Map);
}

bool DefaultScene::Update(const float& delta)
{
	this->m_Player->Update(delta);

	const Vector3f& position = this->m_Player->GetPosition();
	this->m_WorldGrid->Update(Scene::m_Direct3D, position.x, position.z);

	if (this->m_State == GameState::Map)
	{
		if(!this->UpdateMap(delta))
		{
			return false;
		}
	}
	else if(this->m_State == GameState::Surface)
	{
		if(!this->UpdateSurface(delta))
		{
			return false;
		}
	}
	return Scene::Update(delta);
}

bool DefaultScene::UpdateMap(const float& delta)
{
	this->m_HoveredCell = nullptr;
	const GridCell *cells = this->m_WorldGrid->GetCells();
	for (int i = 0; i < this->m_WorldGrid->GetCellCount(); i++)
	{
		const GridCell& cell = cells[i];
		if (cell.IsHovered(float(Scene::m_Input->GetMouseX()), float(Scene::m_Input->GetMouseY()), this->m_LastProjection, this->m_LastView))
		{
			this->m_HoveredCell = &cells[i];
			this->m_IslandHover->SetScale(VECTOR3_SPLIT(cell.m_Island->GetScale() * 1.25f));
			this->m_IslandHover->SetPosition(VECTOR3_SPLIT(cell.m_Island->GetPosition()));
		}
	}

	Camera *camera = Scene::GetCamera();
	if (Scene::m_Input->IsKeyDown(VK_W))
	{
		camera->Translate(0.0f, 0.0f, CAMERA_SPEED * delta);
	}
	if (Scene::m_Input->IsKeyDown(VK_A))
	{
		camera->Translate(CAMERA_SPEED * delta, 0.0f, 0.0f);
	}
	if (Scene::m_Input->IsKeyDown(VK_S))
	{
		camera->Translate(0.0f, 0.0f, -CAMERA_SPEED * delta);
	}
	if (Scene::m_Input->IsKeyDown(VK_D))
	{
		camera->Translate(-CAMERA_SPEED * delta, 0.0f, 0.0f);
	}
	if (Scene::m_Input->IsKeyDown(VK_Q))
	{
		camera->Translate(0.0f, -CAMERA_SPEED * delta, 0.0f);
		camera->SetPosition(camera->GetX(), max(camera->GetY(), MAP_MIN_ZOOM), camera->GetZ());
	}
	if (Scene::m_Input->IsKeyDown(VK_E))
	{
		camera->Translate(0.0f, CAMERA_SPEED * delta, 0.0f);
		camera->SetPosition(camera->GetX(), min(camera->GetY(), MAP_MAX_ZOOM), camera->GetZ());
	}
	if (Scene::m_Input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}
	this->m_Player->SetPosition(VECTOR3_SPLIT(camera->GetPosition()));

	if (this->m_HoveredCell != nullptr && Scene::m_Input->IsMouseDown(MouseButton::LEFT))
	{
		this->m_bTransitioning = true;
		this->m_bSurfaceTransition = true;
		this->SetState(GameState::Surface);

		Vector3f islandPos = this->m_HoveredCell->m_Island->GetPosition();
		this->m_HoveredCell->m_Island->GetSurfaceY(islandPos.x, islandPos.z, islandPos.y);
		((CameraTransition*)camera)->Transition(islandPos + Vector3f(0.0f, -PLAYER_HEIGHT, 0.0f), Vector3f(180.0f, 0.0f, 0.0f), 2.0f);
	}
	return true;
}

bool DefaultScene::UpdateSurface(const float& delta)
{
	CameraTransition *camera = (CameraTransition*)Scene::GetCamera();
	if (this->m_bTransitioning)
	{
		if (camera->IsTransitionComplete())
		{
			this->m_bTransitioning = false;
			if (!this->m_bSurfaceTransition)
			{
				this->SetState(GameState::Map);
			}
		}
	}
	else
	{
		Vector3f previousPosition = camera->GetPosition();
		if (Scene::m_Input->IsKeyDown(VK_W))
		{
			camera->MoveForward(CAMERA_SURFACE_SPEED * delta);
		}
		if (Scene::m_Input->IsKeyDown(VK_A))
		{
			camera->MoveSideways(-CAMERA_SURFACE_SPEED * delta);
		}
		if (Scene::m_Input->IsKeyDown(VK_S))
		{
			camera->MoveForward(-CAMERA_SURFACE_SPEED * delta);
		}
		if (Scene::m_Input->IsKeyDown(VK_D))
		{
			camera->MoveSideways(CAMERA_SURFACE_SPEED * delta);
		}
		camera->Rotate(Scene::GetCameraDX() * delta, Scene::GetCameraDY() * delta);

		float outY; 
		if(this->m_HoveredCell->m_Island->GetSurfaceY(camera->GetX(), camera->GetZ(), outY))
		{
			camera->SetPosition(camera->GetX(), outY - PLAYER_HEIGHT, camera->GetZ());
		}
		else
		{
			camera->SetPosition(VECTOR3_SPLIT(previousPosition));
		}

		if (Scene::m_Input->IsKeyDown(VK_ESCAPE))
		{
			this->m_bTransitioning = true;
			this->m_bSurfaceTransition = false;

			SystemClass::SetMouseGrab(false);

			const Vector3f& pos = camera->GetPosition();
			camera->Transition(Vector3f(pos.x, MAP_CAMERA_Y, pos.z), Vector3f(90.0f, 0.0f, 0.0f), 1.25f);
		}
	}
	return true;
}

void DefaultScene::Render(D3DClass* direct, const D3DXMATRIX& projection)
{
	Camera *camera = Scene::GetCamera();
	if (camera == nullptr)
	{
		return;
	}
	const D3DXMATRIX& view = camera->GetViewMatrix();
	this->m_WorldGrid->Render(direct, projection, view);
	if (this->m_HoveredCell != nullptr && this->m_State == GameState::Map)
	{
		this->m_IslandHover->Render(direct, projection, view);
	}

	this->m_LastView = view;
	this->m_LastProjection = projection;
}
