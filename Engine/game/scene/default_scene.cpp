#include "default_scene.h"

#include "camera/camera_axis.h"
#include "../world/world_grid.h"
#include "../entity/player.h"
#include "../../inputclass.h"
#include "../../textureshaderclass.h"
#include "../model/generated/island_hover_model.h"
#include "../../systemclass.h"
#include "camera/camera_fpv.h"
#include "camera/camera_transition.h"
#include <cassert>

#define GRID_SIZE 9
#define GRID_WORLD_SIZE 20000.0f

#define CAMERA_SPEED 20000.0f

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
	CameraAxis *camera = new CameraAxis;
	camera->Translate(0.0f, 0.0f, -GRID_WORLD_SIZE * 5.0f);
	Scene::SetCamera(camera);
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
}

void DefaultScene::Update(const float& delta)
{
	this->m_Player->Update(delta);

	const Vector3f& position = this->m_Player->GetPosition();
	this->m_WorldGrid->Update(Scene::m_Direct3D, position.x, position.y);

	InputClass *input = Scene::m_Input;

	this->m_HoveredCell = nullptr;
	const GridCell *cells = this->m_WorldGrid->GetCells();
	for (int i = 0; i < this->m_WorldGrid->GetCellCount(); i++)
	{
		const GridCell& cell = cells[i];
		if (cell.IsHovered(float(input->GetMouseX()), float(input->GetMouseY()), this->m_LastProjection, this->m_LastView))
		{
			this->m_HoveredCell = &cells[i];
			this->m_IslandHover->SetScale(VECTOR3_SPLIT(cell.m_Model->GetScale() * 1.25f));
			this->m_IslandHover->SetPosition(VECTOR3_SPLIT(cell.m_Model->GetPosition()));
		}
	}
	if (this->m_State == GameState::Map)
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
		if (input->IsKeyDown(VK_D))
		{
			camera->Translate(CAMERA_SPEED * delta, 0.0f, 0.0f);
		}
		if (input->IsKeyDown(VK_Q))
		{
			camera->Translate(0.0f, 0.0f, -CAMERA_SPEED * delta);
		}
		if (input->IsKeyDown(VK_E))
		{
			camera->Translate(0.0f, 0.0f, CAMERA_SPEED * delta);
		}
		this->m_Player->SetPosition(VECTOR3_SPLIT(camera->GetPosition()));

		if (this->m_HoveredCell != nullptr && input->IsMouseDown(MouseButton::LEFT))
		{
			this->m_bTransitioning = true;
			this->SetState(GameState::Surface);

			Camera *current = Scene::GetCamera();

			const float h = 1.75f;
			Vector3f start = current->GetPosition(), end = this->m_HoveredCell->m_Model->GetPosition() - Vector3f(0.0f, 0.0f, h);

			Vector3f normal = (end - start).normalize();
			Vector3f lookAt = end + normal * sqrtf(h * h + h * h) - Vector3f(0.0f, 0.0f, h);

			CameraTransition *transition = new CameraTransition(start, current->GetLookAt(), end, lookAt, 2.0f);
			Scene::SetCamera(transition);
		}
	}
	else if(this->m_State == GameState::Surface)
	{
		if(this->m_bTransitioning)
		{
			CameraTransition *camera = (CameraTransition*)Scene::GetCamera();
			if(camera->IsTransitionComplete())
			{
				CameraFPV *fpv = new CameraFPV;
				fpv->SetPosition(VECTOR3_SPLIT(camera->GetPosition()));
				//Scene::SetCamera(fpv);

				this->m_bTransitioning = false;
			}
		}
		else
		{
			//SystemClass::DebugOut(L"Done\n");
		}
	}

	Scene::Update(delta);
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
	if (this->m_HoveredCell != nullptr)
	{
		this->m_IslandHover->Render(direct, projection, view);
	}

	this->m_LastView = view;
	this->m_LastProjection = projection;
}
