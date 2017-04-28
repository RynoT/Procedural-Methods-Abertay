#include "post_processor.h"

#include "post_effect.h"
#include "../../util/math/vector3f.h"
#include "../../../orthowindowclass.h"
#include "../../../applicationclass.h"
#include "../../../rendertextureclass.h"
#include "no_effect.h"

PostProcessor::PostProcessor(D3DClass* direct, const HWND& hwnd, TextureShaderClass *shader)
	: m_RenderTexture(nullptr), m_TextureShader(shader), m_Window(nullptr), m_SmallWindow(nullptr)
{
	D3DXVECTOR3 position = Vector3f(0.0f, 0.0f, -1.0f);
	D3DXVECTOR3 lookAt = position + Vector3f(0.0f, 0.0f, 1.0f);
	D3DXVECTOR3 up = Vector3f(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&this->m_WindowViewMatrix, &position, &lookAt, &up);
	
	this->OnResize(direct, ApplicationClass::SCREEN_WIDTH, ApplicationClass::SCREEN_HEIGHT);

	this->m_NoEffect = new NoEffect(direct, hwnd);
}

PostProcessor::~PostProcessor()
{
	for (PostEffect *effect : this->m_Effects)
	{
		delete effect;
	}
	this->m_Effects.empty();

	if (this->m_NoEffect != nullptr)
	{
		delete this->m_NoEffect;
		this->m_NoEffect = nullptr;
	}
	if (this->m_RenderTexture != nullptr)
	{
		this->m_RenderTexture->Shutdown();
		delete this->m_RenderTexture;
		this->m_RenderTexture = nullptr;
	}
	if (this->m_Window != nullptr)
	{
		this->m_Window->Shutdown();
		delete this->m_Window;
		this->m_Window = nullptr;
	}
	if (this->m_SmallWindow != nullptr)
	{
		this->m_SmallWindow->Shutdown();
		delete this->m_SmallWindow;
		this->m_SmallWindow = nullptr;
	}
}

void PostProcessor::OnResize(D3DClass* d3d, const int& width, const int& height)
{
	if (this->m_Window != nullptr)
	{
		this->m_Window->Shutdown();
		delete this->m_Window;
	}
	if (this->m_SmallWindow != nullptr)
	{
		this->m_SmallWindow->Shutdown();
		delete this->m_SmallWindow;
	}
	if (this->m_RenderTexture != nullptr)
	{
		this->m_RenderTexture->Shutdown();
		delete this->m_RenderTexture;
	}

	this->m_Window = new OrthoWindowClass;
	this->m_Window->Initialize(d3d->GetDevice(), width, height);

	this->m_SmallWindow = new OrthoWindowClass;
	this->m_SmallWindow->Initialize(d3d->GetDevice(), DOWN_SAMPLE_WIDTH, DOWN_SAMPLE_HEIGHT);

	this->m_RenderTexture = new RenderTextureClass;
	this->m_RenderTexture->Initialize(d3d->GetDevice(), width, height, SCREEN_DEPTH, SCREEN_NEAR);

	for (PostEffect *effect : this->m_Effects)
	{
		effect->OnResize(d3d, width, height);
	}
}

void PostProcessor::Update(const float& delta)
{
	for (PostEffect *effect : this->m_Effects)
	{
		effect->Update(delta);
	}
}

void PostProcessor::Render(D3DClass* direct)
{
	int count = 0;
	for (PostEffect *effect : this->m_Effects)
	{
		if(effect->Render(this, direct))
		{
			count++;
		}
	}
	// If no post-processing effect activated
	if(count == 0)
	{
		this->m_NoEffect->Render(this, direct);
	}
}
