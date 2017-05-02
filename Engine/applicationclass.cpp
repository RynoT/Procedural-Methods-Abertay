#include "applicationclass.h"

#include "game/scene/default_scene.h"
#include "game/scene/dungeon_scene.h"

int ApplicationClass::SCREEN_WIDTH = 800;
int ApplicationClass::SCREEN_HEIGHT = 600;

ApplicationClass::ApplicationClass(InputClass *input) : m_HWND(nullptr), m_Direct3D(nullptr), m_Scene(nullptr), m_Input(input), m_Timer(nullptr)
{
}

bool ApplicationClass::Initialize(HINSTANCE hinstance, HWND& hwnd, int screenWidth, int screenHeight)
{
	bool result;
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

	// Retrieve the video card information.
	m_Direct3D->GetVideoCardInfo(videoCard, videoMemory);

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
	// Release the timer object.
	if (m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
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

void ApplicationClass::SetScene(const SceneId& id)
{
	if (this->m_Scene != nullptr)
	{
		if (this->m_Scene->GetSceneId() == id)
		{
			this->m_Scene->OnRefresh(this->m_Direct3D);
			return;
		}
		delete this->m_Scene;
	}
	Scene *scene = nullptr;
	switch (id)
	{
		case SceneId::DEFAULT:
			scene = new DefaultScene(this->m_Direct3D, *this->m_HWND, this->m_Input);
			break;
		case SceneId::DUNGEON:
			scene = new DungeonScene(this->m_Direct3D, *this->m_HWND, this->m_Input);
			break;
		default:break;
	}
	this->m_Scene = scene;
}

void ApplicationClass::OnResize(const int& width, const int& height) const
{
	this->m_Direct3D->Resize(width, height);
	if (this->m_Scene != nullptr)
	{
		this->m_Scene->OnResize(this->m_Direct3D, width, height);
	}
}

bool ApplicationClass::Frame()
{
	bool result;

	// Update the system stats.
	m_Timer->Frame();

	if (this->m_Input->IsKeyPressed(VK_NUMPAD1) || this->m_Input->IsKeyPressed(VK_Z))
	{
		this->SetScene(SceneId::DEFAULT);
	}
	else if (this->m_Input->IsKeyPressed(VK_NUMPAD2) || this->m_Input->IsKeyPressed(VK_X))
	{
		this->SetScene(SceneId::DUNGEON);
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

	//this->m_Direct3D->BeginScene(0.15f, 0.16f, 0.17f, 1.0f);
	this->m_Direct3D->BeginScene(CLEAR_COLOR);

	if (this->m_Scene != nullptr)
	{
		this->m_Scene->Render(this->m_Direct3D, projection);
	}

	this->m_Direct3D->EndScene();

	return true;
}
