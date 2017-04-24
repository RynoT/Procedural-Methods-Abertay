#pragma once

#include "model_mesh.h"
#include "../../d3dclass.h"
#include "../../textureclass.h"

struct VertexTypeTexture
{
	D3DXVECTOR3 position;
	D3DXVECTOR2 texture;
};

struct VertexTypeColor
{
	D3DXVECTOR3 position;
	D3DXVECTOR4 color;
};

class Model
{
public:
	Model();
	virtual ~Model();

	// Initialize vertex buffer and index buffer. ModelData should be set before calling
	virtual bool Initialize(ID3D11Device *device) = 0;

	bool InitializeVerticesColor(ID3D11Device *device);

	bool InitializeVerticesTexture(ID3D11Device *device);

	bool InitializeIndices(ID3D11Device *device);

	void Render(ID3D11DeviceContext *context) const;

	inline void SetCollision(Collision *collision) const { this->m_Mesh->SetCollision(collision); }

	inline const Collision* GetCollision() const { return this->m_Mesh->GetCollision(); }

	inline const int& GetIndexCount() const { return this->m_IndexCount; }

	inline const ModelMesh* GetModelMesh() const { return this->m_Mesh; }

	inline ID3D11ShaderResourceView* GetTexture() const { return this->m_Texture->GetTexture(); }

protected:
	virtual ModelMesh* CreateMesh();

	virtual bool LoadModelFromFile(char *path);

	virtual bool LoadTexture(ID3D11Device *device, WCHAR *path);

private:
	ModelMesh *m_Mesh;
	TextureClass *m_Texture;

	int m_VertexCount, m_IndexCount, m_Stride;
	ID3D11Buffer *m_VertexBuffer, *m_IndexBuffer;
};