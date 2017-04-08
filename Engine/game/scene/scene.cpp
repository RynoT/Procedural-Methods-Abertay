#include "scene.h"

Scene::Scene(D3DClass *d3d, const HWND& hwnd, InputClass* input) : m_Direct3D(d3d), m_Input(input)
{
}

Scene::~Scene()
{
}