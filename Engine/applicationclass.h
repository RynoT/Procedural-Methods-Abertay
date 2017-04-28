////////////////////////////////////////////////////////////////////////////////
// Filename: applicationclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _APPLICATIONCLASS_H_
#define _APPLICATIONCLASS_H_


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
#include "inputclass.h"
#include "d3dclass.h"
#include "terrainclass.h"
#include "colorshaderclass.h"
#include "timerclass.h"
#include "fpsclass.h"
#include "cpuclass.h"
#include "fontshaderclass.h"
#include "textclass.h"

class Scene;

class ApplicationClass
{
	friend class SystemClass;

public:
	ApplicationClass(InputClass *input);
	~ApplicationClass();

	virtual void OnResize(const int& width, const int& height);

	bool Initialize(HINSTANCE, HWND&, int, int);
	void Shutdown();
	bool Frame();

	void SetScene(class Scene *scene);

	static int SCREEN_WIDTH;
	static int SCREEN_HEIGHT;

private:
	bool RenderGraphics() const;

private:
	HWND *m_HWND;
	D3DClass* m_Direct3D;
	InputClass *m_Input;
	TerrainClass* m_Terrain;
	ColorShaderClass* m_ColorShader;
	TimerClass* m_Timer;
	FpsClass* m_Fps;
	CpuClass* m_Cpu;
	FontShaderClass* m_FontShader;
	TextClass* m_Text;

	Scene *m_Scene;
};

#endif