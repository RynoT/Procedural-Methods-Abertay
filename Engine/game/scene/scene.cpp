#include "scene.h"

#include "camera/camera.h"

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

void Scene::SetCamera(Camera* camera)
{
	if(this->m_Camera != nullptr)
	{
		delete this->m_Camera;
	}
	this->m_Camera = camera;
}

void Scene::Update(const float& delta)
{
	if(this->m_Camera != nullptr)
	{
		this->m_Camera->Update(delta);
	}
}
