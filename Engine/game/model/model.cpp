#include "model.h"

#include <fstream>

Model::Model() : m_Mesh(nullptr), m_Texture(nullptr), m_VertexCount(0),
m_IndexCount(0), m_VertexBuffer(nullptr), m_IndexBuffer(nullptr)
{
}

Model::~Model()
{
	if (this->m_IndexBuffer != nullptr)
	{
		this->m_IndexBuffer->Release();
		this->m_IndexBuffer = nullptr;
	}
	if (this->m_VertexBuffer)
	{
		this->m_VertexBuffer->Release();
		this->m_VertexBuffer = nullptr;
	}
	if (this->m_Texture != nullptr)
	{
		this->m_Texture->Shutdown();
		delete m_Texture;
		this->m_Texture = nullptr;
	}
	if (this->m_Mesh != nullptr)
	{
		delete this->m_Mesh;
		this->m_Mesh = nullptr;
	}
}

void Model::Render(ID3D11DeviceContext* context) const
{
	// Set vertex buffer stride and offset.
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	context->IASetVertexBuffers(0, 1, &this->m_VertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	context->IASetIndexBuffer(this->m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool Model::Initialize(ID3D11Device *device)
{
	if(this->m_Mesh == nullptr)
	{
		return false;
	}

	HRESULT result;
	VertexType *vertices = new VertexType[this->m_VertexCount];
	unsigned long *indices = new unsigned long[this->m_IndexCount];

	// Load the vertex array and index array with data.
	ModelData *data = this->m_Mesh->GetModelData();
	for (int i = 0; i < this->m_VertexCount; i++)
	{
		vertices[i].position = D3DXVECTOR3(data[i].x, data[i].y, data[i].z);
		vertices[i].texture = D3DXVECTOR2(data[i].tu, data[i].tv);

		indices[i] = i;
	}

	D3D11_BUFFER_DESC vertexBufferDesc;
	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * this->m_VertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &this->m_VertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	D3D11_BUFFER_DESC indexBufferDesc;
	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * this->m_IndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &this->m_IndexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	delete[] vertices;
	delete[] indices;

	return true;
}

bool Model::LoadTexture(ID3D11Device* device, WCHAR* path)
{
	if (this->m_Texture != nullptr)
	{
		this->m_Texture->Shutdown();
		delete this->m_Texture;
	}
	this->m_Texture = new TextureClass;
	return this->m_Texture->Initialize(device, path);
}


bool Model::LoadModelFromFile(char* path)
{
	// Open the model file.  If it could not open the file then exit.
	std::ifstream fin;
	fin.open(path);
	if (fin.fail())
	{
		return false;
	}

	// Read up to the value of vertex count.
	char input;
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	// Read in the vertex count.
	fin >> this->m_VertexCount;

	// Set the number of indices to be the same as the vertex count.
	this->m_IndexCount = this->m_VertexCount;

	// Create the model using the vertex count that was read in.
	this->m_Mesh = new ModelMesh(this->m_VertexCount);

	// Read up to the beginning of the data.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the vertex data.
	ModelData *data = this->m_Mesh->GetModelData();
	for (int i = 0; i < this->m_VertexCount; i++)
	{
		fin >> data[i].x >> data[i].y >> data[i].z;
		fin >> data[i].tu >> data[i].tv;
		fin >> data[i].nx >> data[i].ny >> data[i].nz;
	}

	// Close the model file.
	fin.close();

	return true;
}