#include "scene.h"

#include "camera/camera.h"
#include "post/blur_effect.h"
#include "post/invert_effect.h"
#include "post/vignette_effect.h"
#include "post/edge_detect_effect.h"
#include "post/convolution_blur_effect.h"
#include "post/post_processor.h"
#include "../../applicationclass.h"
#include "../../textureshaderclass.h"
#include "../../rendertextureclass.h"

Scene::Scene(const SceneId& id, D3DClass *d3d, const HWND& hwnd, InputClass* input) 
: m_Direct3D(d3d), m_Input(input), m_Id(id), m_Camera(nullptr)
{
	this->m_TextureShader = new TextureShaderClass;
	this->m_TextureShader->Initialize(d3d->GetDevice(), hwnd);

	// Setup post processing
	this->m_PostProcessor = new PostProcessor(d3d, hwnd, this->m_TextureShader);
	this->m_PostProcessor->AddEffect(this->m_EdgeEffect = new EdgeDetectEffect(d3d, hwnd));
	this->m_PostProcessor->AddEffect(this->m_ConvBlurEffect = new ConvolutionBlurEffect(d3d, hwnd));
	this->m_PostProcessor->AddEffect(this->m_BlurEffect = new BlurEffect(d3d, hwnd));
	this->m_PostProcessor->AddEffect(this->m_InvertEffect = new InvertEffect(d3d, hwnd));
	this->m_PostProcessor->AddEffect(this->m_VignatteEffect = new VignetteEffect(d3d, hwnd));
	// Force the post processor to update the dimensions of all effects. Only happens if the current dimensions do not match.
	this->m_PostProcessor->OnResize(d3d, ApplicationClass::SCREEN_WIDTH, ApplicationClass::SCREEN_HEIGHT);

	// Disable these effects. The rest are enabled by default
	this->m_BlurEffect->SetEnabled(false);
	this->m_EdgeEffect->SetEnabled(false);
	this->m_InvertEffect->SetEnabled(false);
	this->m_ConvBlurEffect->SetEnabled(false);
}

Scene::~Scene()
{
	if (this->m_Camera != nullptr)
	{
		delete this->m_Camera;
		this->m_Camera = nullptr;
	}
	if (this->m_PostProcessor != nullptr)
	{
		delete this->m_PostProcessor;
		this->m_PostProcessor = nullptr;
	}
	if (this->m_TextureShader != nullptr)
	{
		this->m_TextureShader->Shutdown();
		delete this->m_TextureShader;
		this->m_TextureShader = nullptr;
	}
}

float Scene::GetCameraDX() const
{
	return (float)this->m_Input->GetMouseX() - (int)(ApplicationClass::SCREEN_WIDTH / 2.0f);
}

float Scene::GetCameraDY() const
{
	return (float)this->m_Input->GetMouseY() - (int)(ApplicationClass::SCREEN_HEIGHT / 2.0f);
}

void Scene::SetCamera(Camera* camera)
{
	if (this->m_Camera != nullptr)
	{
		delete this->m_Camera;
	}
	this->m_Camera = camera;
}

void Scene::OnResize(D3DClass *d3d, const int& width, const int& height)
{
	this->m_PostProcessor->OnResize(d3d, width, height);
}

bool Scene::Update(const float& delta)
{
	// Enabled/Disable post effects
	if (Scene::m_Input->IsKeyPressed(VK_F1))
	{
		this->m_BlurEffect->ToggleEnabled();
	}
	if (Scene::m_Input->IsKeyPressed(VK_F2))
	{
		this->m_VignatteEffect->ToggleEnabled();
	}
	if (Scene::m_Input->IsKeyPressed(VK_F3))
	{
		this->m_InvertEffect->ToggleEnabled();
	}
	if (Scene::m_Input->IsKeyPressed(VK_F4))
	{
		this->m_EdgeEffect->ToggleEnabled();
	}
	if (Scene::m_Input->IsKeyPressed(VK_F5))
	{
		this->m_ConvBlurEffect->ToggleEnabled();
	}
	if (this->m_Input != nullptr)
	{
		this->m_Input->Update();
	}
	if (this->m_Camera != nullptr)
	{
		this->m_Camera->Update(delta);
	}
	return true;
}

void Scene::Render(D3DClass* direct, const D3DXMATRIX& projection)
{
	this->m_PostProcessor->GetSceneRenderTexture()->SetRenderTarget(direct->GetDeviceContext());
	this->m_PostProcessor->GetSceneRenderTexture()->ClearRenderTarget(direct->GetDeviceContext(), CLEAR_COLOR);

	this->RenderScene(direct, projection);

	this->m_PostProcessor->Render(direct);
}
