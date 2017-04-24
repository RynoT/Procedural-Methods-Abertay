#include "applicationclass.h"

#include "game/scene/default_scene.h"
#include "game/scene/dungeon_scene.h"

int ApplicationClass::SCREEN_WIDTH = 800;
int ApplicationClass::SCREEN_HEIGHT = 600;

ApplicationClass::ApplicationClass(InputClass *input) : m_Input(input), m_Scene(nullptr)
{
	m_Direct3D = 0;
	m_Camera = 0;
	m_Terrain = 0;
	m_ColorShader = 0;
	m_Timer = 0;
	m_Position = 0;
	m_Fps = 0;
	m_Cpu = 0;
	m_FontShader = 0;
	m_Text = 0;
}

ApplicationClass::~ApplicationClass()
{
}

bool ApplicationClass::Initialize(HINSTANCE hinstance, HWND& hwnd, int screenWidth, int screenHeight)
{
	bool result;
	float cameraX, cameraY, cameraZ;
	D3DXMATRIX baseViewMatrix;
	char videoCard[128];
	int videoMemory;

	this->m_HWND = &hwnd;

	// Create the Direct3D object.
	m_Direct3D = new D3DClass;
	if (!m_Direct3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize DirectX 11.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// Initialize a base view matrix with the camera for 2D user interface rendering.
	m_Camera->SetPosition(0.0f, 0.0f, -1.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	// Set the initial position of the camera.
	cameraX = 50.0f;
	cameraY = 2.0f;
	cameraZ = -7.0f;

	m_Camera->SetPosition(cameraX, cameraY, cameraZ);

	// Create the terrain object.
	m_Terrain = new TerrainClass;
	if (!m_Terrain)
	{
		return false;
	}

	// Initialize the terrain object.
	result = m_Terrain->Initialize(m_Direct3D->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain object.", L"Error", MB_OK);
		return false;
	}

	// Create the color shader object.
	m_ColorShader = new ColorShaderClass;
	if (!m_ColorShader)
	{
		return false;
	}

	// Initialize the color shader object.
	result = m_ColorShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the timer object.
	m_Timer = new TimerClass;
	if (!m_Timer)
	{
		return false;
	}

	// Initialize the timer object.
	result = m_Timer->Initialize();
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the timer object.", L"Error", MB_OK);
		return false;
	}

	// Create the position object.
	m_Position = new PositionClass;
	if (!m_Position)
	{
		return false;
	}

	// Set the initial position of the viewer to the same as the initial camera position.
	m_Position->SetPosition(cameraX, cameraY, cameraZ);

	// Create the fps object.
	m_Fps = new FpsClass;
	if (!m_Fps)
	{
		return false;
	}

	// Initialize the fps object.
	m_Fps->Initialize();

	// Create the cpu object.
	m_Cpu = new CpuClass;
	if (!m_Cpu)
	{
		return false;
	}

	// Initialize the cpu object.
	m_Cpu->Initialize();

	// Create the font shader object.
	m_FontShader = new FontShaderClass;
	if (!m_FontShader)
	{
		return false;
	}

	// Initialize the font shader object.
	result = m_FontShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the font shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the text object.
	m_Text = new TextClass;
	if (!m_Text)
	{
		return false;
	}

	// Initialize the text object.
	result = m_Text->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	// Retrieve the video card information.
	m_Direct3D->GetVideoCardInfo(videoCard, videoMemory);

	// Set the video card information in the text object.
	result = m_Text->SetVideoCardInfo(videoCard, videoMemory, m_Direct3D->GetDeviceContext());
	if (!result)
	{
		MessageBox(hwnd, L"Could not set video card info in the text object.", L"Error", MB_OK);
		return false;
	}

	this->m_Scene = new DefaultScene(this->m_Direct3D, hwnd, this->m_Input);
	//this->m_Scene = new DungeonScene(this->m_Direct3D, hwnd, this->m_Input);

	if (WIREFRAME_MODE)
	{
		this->m_Direct3D->TurnOnWireframe();
	}

	return true;
}


void ApplicationClass::Shutdown()
{
	// Release the text object.
	if (m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	// Release the font shader object.
	if (m_FontShader)
	{
		m_FontShader->Shutdown();
		delete m_FontShader;
		m_FontShader = 0;
	}

	// Release the cpu object.
	if (m_Cpu)
	{
		m_Cpu->Shutdown();
		delete m_Cpu;
		m_Cpu = 0;
	}

	// Release the fps object.
	if (m_Fps)
	{
		delete m_Fps;
		m_Fps = 0;
	}

	// Release the position object.
	if (m_Position)
	{
		delete m_Position;
		m_Position = 0;
	}

	// Release the timer object.
	if (m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
	}

	// Release the color shader object.
	if (m_ColorShader)
	{
		m_ColorShader->Shutdown();
		delete m_ColorShader;
		m_ColorShader = 0;
	}

	// Release the terrain object.
	if (m_Terrain)
	{
		m_Terrain->Shutdown();
		delete m_Terrain;
		m_Terrain = 0;
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the Direct3D object.
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}

	if (this->m_Scene != nullptr)
	{
		delete this->m_Scene;
		this->m_Scene = nullptr;
	}
}

void ApplicationClass::SetScene(Scene* scene)
{
	if(this->m_Scene != nullptr)
	{
		delete this->m_Scene;
	}
	this->m_Scene = scene;
}

bool ApplicationClass::Frame()
{
	bool result;

	// Update the system stats.
	m_Timer->Frame();
	m_Fps->Frame();
	m_Cpu->Frame();

	// Update the FPS value in the text object.
	result = m_Text->SetFps(m_Fps->GetFps(), m_Direct3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	// Update the CPU usage value in the text object.
	result = m_Text->SetCpu(m_Cpu->GetCpuPercentage(), m_Direct3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	if(this->m_Input->IsKeyPressed(VK_NUMPAD1))
	{
		this->SetScene(new DefaultScene(this->m_Direct3D, *this->m_HWND, this->m_Input));
	}
	else if(this->m_Input->IsKeyPressed(VK_NUMPAD2))
	{
		this->SetScene(new DungeonScene(this->m_Direct3D, *this->m_HWND, this->m_Input));
	}

	if (this->m_Scene != nullptr)
	{
		result = this->m_Scene->Update(this->m_Timer->GetTime());
		if (!result)
		{
			return false;
		}
	}

	// Render the graphics.
	result = RenderGraphics();
	if (!result)
	{
		return false;
	}

	return result;
}

bool ApplicationClass::RenderGraphics() const
{
	D3DXMATRIX projection;
	this->m_Direct3D->GetProjectionMatrix(projection);

	this->m_Direct3D->BeginScene(0.15f, 0.16f, 0.17f, 1.0f);

	if (this->m_Scene != nullptr)
	{
		this->m_Scene->Render(this->m_Direct3D, projection);
	}

	this->m_Direct3D->EndScene();

	return true;
}
