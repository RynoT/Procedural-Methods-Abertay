#pragma once

#include "../../d3dclass.h"

class Camera;
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
	void SetCamera(Camera *camera);

	D3DXMATRIX& GetViewMatrix();

	D3DClass *m_Direct3D;
	InputClass *m_Input;

private:
	Camera *m_Camera;
	D3DXMATRIX m_ViewMatrix;
};