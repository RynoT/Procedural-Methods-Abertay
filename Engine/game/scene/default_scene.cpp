#include "default_scene.h"

#include "../entity/player.h"
#include "../world/world_grid.h"
#include "../../textureshaderclass.h"

#define GRID_WORLD_SIZE 50.0f

DefaultScene::DefaultScene(D3DClass *d3d, const HWND& hwnd, InputClass *input) : Scene(d3d, hwnd, input)
{	
	this->m_TextureShader = new TextureShaderClass;
	this->m_TextureShader->Initialize(d3d->GetDevice(), hwnd);

	this->m_Player = new Player;

	this->m_WorldGrid = new WorldGrid(GRID_WORLD_SIZE);
	this->m_WorldGrid->SetShader(this->m_TextureShader);
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

void DefaultScene::Update(const float& delta)
{
	this->m_Player->Update(delta);

	const D3DXVECTOR3& position = this->m_Player->GetPosition();
	this->m_WorldGrid->Update(Scene::m_Direct3D, position.x, position.z);
}

void DefaultScene::Render(D3DClass* direct, D3DXMATRIX& projection)
{
}
