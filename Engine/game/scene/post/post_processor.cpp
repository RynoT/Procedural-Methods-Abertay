#include "post_processor.h"

#include "post_effect.h"
#include "../../util/math/vector3f.h"
#include "../../../orthowindowclass.h"
#include "../../../applicationclass.h"
#include "../../../textureshaderclass.h"
#include "../../../rendertextureclass.h"

PostProcessor::PostProcessor(D3DClass* direct, const HWND& hwnd, TextureShaderClass *shader)
	: m_LastWidth(-1), m_LastHeight(-1), m_RenderTexture(nullptr), m_TextureShader(shader), m_Window(nullptr), m_SmallWindow(nullptr)
{
	D3DXVECTOR3 position = Vector3f(0.0f, 0.0f, -1.0f);
	D3DXVECTOR3 lookAt = position + Vector3f(0.0f, 0.0f, 1.0f);
	D3DXVECTOR3 up = Vector3f(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&this->m_WindowViewMatrix, &position, &lookAt, &up);
}

PostProcessor::~PostProcessor()
{
	for (PostEffect *effect : this->m_Effects)
	{
		delete effect;
	}
	this->m_Effects.empty();

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
	if(this->m_LastWidth == width && this->m_LastHeight == height)
	{
		return;
	}
	this->m_LastWidth = width;
	this->m_LastHeight = height;

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
	if(this->m_LastWidth <= 0 || this->m_LastHeight <= 0)
	{
		return;
	}
	D3DXMATRIX world, view = this->m_WindowViewMatrix;
	direct->GetWorldMatrix(world);

	for (PostEffect *effect : this->m_Effects)
	{
		effect->Render(this, direct, world, view);
	}

	direct->SetBackBufferRenderTarget();
	direct->ResetViewport();

	D3DXMATRIX orthoMatrix;
	direct->GetOrthoMatrix(orthoMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	direct->TurnZBufferOff();

	// Put the full screen ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing.
	this->GetWindow()->Render(direct->GetDeviceContext());

	// Render the full screen ortho window using the texture shader and the full screen sized blurred render to texture resource.
	this->GetTextureShader()->Render(direct->GetDeviceContext(), this->GetWindow()->GetIndexCount(),
		world, view, orthoMatrix, this->m_RenderTexture->GetShaderResourceView());

	// Turn the Z buffer back on now that all 2D rendering has completed.
	direct->TurnZBufferOn();
}
