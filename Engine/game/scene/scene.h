#pragma once

#include "../../d3dclass.h"

class D3DClass;
class InputClass;

class Scene
{
protected:
	Scene(D3DClass *d3d, const HWND& hwnd, InputClass *input);

public:
	virtual ~Scene();

	virtual void Update(const float& delta) = 0;

	virtual void Render(D3DClass* direct, D3DXMATRIX& projection) = 0;

protected:
	D3DClass *m_Direct3D;

	InputClass *m_Input;
};