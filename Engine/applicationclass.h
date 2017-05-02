////////////////////////////////////////////////////////////////////////////////
// Filename: applicationclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _APPLICATIONCLASS_H_
#define _APPLICATIONCLASS_H_
#include "game/scene/scene.h"


/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const bool SHOW_CURSOR = true;
const float SCREEN_DEPTH = 1000000.0f;
const float SCREEN_NEAR = 0.1f;
const bool WIREFRAME_MODE = false;

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "inputclass.h"
#include "timerclass.h"

#define CLEAR_COLOR 0.05f, 0.18f, 0.18f, 1.0f

class ApplicationClass
{
	friend class SystemClass;

public:
	ApplicationClass(InputClass *input);

	void OnResize(const int& width, const int& height) const;

	bool Initialize(HINSTANCE, HWND&, int, int);
	void Shutdown();
	bool Frame();

	void SetScene(const SceneId& id);

	static int SCREEN_WIDTH;
	static int SCREEN_HEIGHT;

private:
	bool RenderGraphics() const;

private:
	HWND *m_HWND;
	D3DClass* m_Direct3D;

	Scene *m_Scene;
	InputClass *m_Input;
	TimerClass* m_Timer;
};

#endif