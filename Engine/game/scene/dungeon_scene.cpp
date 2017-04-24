#include "dungeon_scene.h"

#include "../../colorshaderclass.h"

DungeonScene::DungeonScene(D3DClass* d3d, const HWND& hwnd, InputClass* input) : Scene(d3d, hwnd, input)
{
	this->m_ColorShader = new ColorShaderClass;
	this->m_ColorShader->Initialize(d3d->GetDevice(), hwnd);
}

DungeonScene::~DungeonScene()
{
	if(this->m_ColorShader != nullptr)
	{
		this->m_ColorShader->Shutdown();
		delete this->m_ColorShader;
		this->m_ColorShader = nullptr;
	}
}

bool DungeonScene::Update(const float& delta)
{
	return Scene::Update(delta);
}

void DungeonScene::Render(D3DClass* direct, const D3DXMATRIX& projection)
{

}
