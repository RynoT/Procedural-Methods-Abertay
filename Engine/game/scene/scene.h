#pragma once

#include "../../d3dclass.h"

class Camera;
class InputClass;

class Scene
{
protected:
	Scene(D3DClass *d3d, const HWND& hwnd, InputClass *input);

public:
	virtual ~Scene();

	virtual void Update(const float& delta);

	virtual void Render(D3DClass* direct, const D3DXMATRIX& projection) = 0;

protected:
	void SetCamera(Camera *camera);

	inline Camera* GetCamera() const { return this->m_Camera; }

	D3DClass *m_Direct3D;
	InputClass *m_Input;

private:
	Camera *m_Camera;
	D3DXMATRIX m_ViewMatrix;
};