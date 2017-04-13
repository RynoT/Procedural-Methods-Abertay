#pragma once

#include "../../d3dclass.h"
#include "../../textureclass.h"

struct VertexType
{
	D3DXVECTOR3 position;
	D3DXVECTOR2 texture;
};

struct ModelData
{
	float x, y, z;
	float tu, tv;
	float nx, ny, nz;
};

class Model
{
public:
	Model();
	virtual ~Model();

	// Initialize vertex buffer and index buffer. ModelData should be set before calling
	virtual bool Initialize(ID3D11Device *device);

	void Render(ID3D11DeviceContext *context) const;

	inline const int& GetIndexCount() const { return this->m_IndexCount; }

	inline ID3D11ShaderResourceView* GetTexture() const { return this->m_Texture->GetTexture(); }

protected:
	virtual bool LoadModelFromFile(char *path);

	virtual bool LoadTexture(ID3D11Device *device, WCHAR *path);

private:
	ModelData *m_ModelData;
	TextureClass *m_Texture;

	int m_VertexCount, m_IndexCount;
	ID3D11Buffer *m_VertexBuffer, *m_IndexBuffer;
};