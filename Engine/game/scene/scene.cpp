#include "scene.h"

#include "camera/camera.h"
#include "../../applicationclass.h"

Scene::Scene(D3DClass *d3d, const HWND& hwnd, InputClass* input) : m_Direct3D(d3d), m_Input(input), m_Camera(nullptr)
{
}

Scene::~Scene()
{
	if (this->m_Camera != nullptr)
	{
		delete this->m_Camera;
		this->m_Camera = nullptr;
	}
}

float Scene::GetCameraDX() const
{
	return (float)this->m_Input->GetMouseX() - (int)(ApplicationClass::SCREEN_WIDTH / 2.0f);
}

float Scene::GetCameraDY() const
{
	return (float)this->m_Input->GetMouseY() - (int)(ApplicationClass::SCREEN_HEIGHT / 2.0f);
}

void Scene::SetCamera(Camera* camera)
{
	if(this->m_Camera != nullptr)
	{
		delete this->m_Camera;
	}
	this->m_Camera = camera;
}

bool Scene::Update(const float& delta)
{
	if(this->m_Camera != nullptr)
	{
		this->m_Camera->Update(delta);
	}
	return true;
}
