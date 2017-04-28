////////////////////////////////////////////////////////////////////////////////
// Filename: systemclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_

///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#define WIN32_LEAN_AND_MEAN


//////////////
// INCLUDES //
//////////////
#include <windows.h>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "inputclass.h"
#include "applicationclass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: SystemClass
////////////////////////////////////////////////////////////////////////////////
class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	void OnResize();
	bool Initialize();
	void Shutdown();
	void Run() const;

	static void SetMouseGrab(const bool& enable);

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

	static void WINAPIV DebugOut(const TCHAR *fmt, ...) 
	{
		TCHAR s[1025];
		va_list args;
		ZeroMemory(s, 1025 * sizeof(s[0]));
		va_start(args, fmt);
		wvsprintf(s, fmt, args);
		va_end(args);
		s[1024] = 0;
		OutputDebugString(s);
	}

private:
	static bool MOUSE_GRAB;

	bool Frame() const;
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;
	RECT m_bounds;

	InputClass* m_Input;
	ApplicationClass* m_Application;
};


/////////////////////////
// FUNCTION PROTOTYPES //
/////////////////////////
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


/////////////
// GLOBALS //
/////////////
static SystemClass* ApplicationHandle = 0;


#endif