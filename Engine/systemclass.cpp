////////////////////////////////////////////////////////////////////////////////
// Filename: systemclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "systemclass.h"

bool SystemClass::MOUSE_GRAB = false;

SystemClass::SystemClass()
{
	m_Input = 0;
	m_Application = 0;
}


SystemClass::SystemClass(const SystemClass& other)
{
}


SystemClass::~SystemClass()
{
}

void SystemClass::SetMouseGrab(const bool& enable)
{
	SystemClass::MOUSE_GRAB = enable;
	if(!enable)
	{
		while (ShowCursor(true) < 0);
	}
	else
	{
		while (ShowCursor(false) >= 0);
	}
}

bool SystemClass::Initialize()
{
	int screenWidth, screenHeight;
	bool result;


	// Initialize the width and height of the screen to zero before sending the variables into the function.
	screenWidth = 0;
	screenHeight = 0;

	// Initialize the windows api.
	InitializeWindows(screenWidth, screenHeight);

	// Create the input object.  This object will be used to handle reading the keyboard input from the user.
	m_Input = new InputClass;
	if (!m_Input)
	{
		return false;
	}

	// Create the graphics object.  This object will handle rendering all the graphics for this application.
	m_Application = new ApplicationClass(this->m_Input);
	if (!m_Application)
	{
		return false;
	}

	// Initialize the graphics object.
	result = m_Application->Initialize(m_hinstance, m_hwnd, screenWidth, screenHeight);
	if (!result)
	{
		return false;
	}
	this->OnResize();

	return true;
}


void SystemClass::Shutdown()
{
	// Release the graphics object.
	if (m_Application)
	{
		m_Application->Shutdown();
		delete m_Application;
		m_Application = 0;
	}

	// Release the input object.
	if (m_Input)
	{
		delete m_Input;
		m_Input = 0;
	}

	// Shutdown the window.
	ShutdownWindows();
}


void SystemClass::Run() const
{
	MSG msg;
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (SystemClass::MOUSE_GRAB)
			{
				POINT point;
				point.x = ApplicationClass::SCREEN_WIDTH / 2;
				point.y = ApplicationClass::SCREEN_HEIGHT / 2;
				ClientToScreen(this->m_hwnd, &point);
				SetCursorPos(point.x, point.y);
			}
			if(!this->Frame())
			{
				break;
			}
		}
	}
}

bool SystemClass::Frame() const
{
	return this->m_Application->Frame();
}

void SystemClass::OnResize()
{
	GetClientRect(this->m_hwnd, &this->m_bounds);
	if (this->m_bounds.bottom == 0)
	{
		this->m_bounds.bottom = 1;
	}
	ApplicationClass::SCREEN_WIDTH = this->m_bounds.right;
	ApplicationClass::SCREEN_HEIGHT = this->m_bounds.bottom;
	if (this->m_Application != nullptr)
	{
		this->m_Application->OnResize(this->m_bounds.right, this->m_bounds.bottom);
	}
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		case WM_SIZE:
			this->OnResize();
			break;
		case WM_KEYDOWN:
			this->m_Input->SetKey(wparam, true);
			break;
		case WM_KEYUP:
			this->m_Input->SetKey(wparam, false);
			break;
		case WM_LBUTTONUP:
			this->m_Input->SetMouseButton(true, false);
			break;
		case WM_RBUTTONUP:
			this->m_Input->SetMouseButton(false, false);
			break;
		case WM_LBUTTONDOWN:
			this->m_Input->SetMouseButton(true, true);
			break;
		case WM_RBUTTONDOWN:
			this->m_Input->SetMouseButton(false, true);
			break;
		case WM_MOUSEMOVE:
			this->m_Input->SetMousePosition(LOWORD(lparam), HIWORD(lparam));
			break;
		default: break;
	}
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}


void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;


	// Get an external pointer to this object.	
	ApplicationHandle = this;

	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	m_applicationName = L"Procedural Generation";

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		screenWidth = ApplicationClass::SCREEN_WIDTH;
		screenHeight = ApplicationClass::SCREEN_HEIGHT;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, m_applicationName, m_applicationName, WS_OVERLAPPEDWINDOW,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	ShowCursor(SHOW_CURSOR);

	return;
}


void SystemClass::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;

	return;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// Check if the window is being destroyed.
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		// Check if the window is being closed.
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}

		// All other messages pass to the message handler in the system class.
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}