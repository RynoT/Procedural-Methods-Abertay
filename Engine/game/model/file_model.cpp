#include "file_model.h"

FileModel::FileModel(char* modelPath, WCHAR* texturePath) : m_ModelPath(modelPath), m_TexturePath(texturePath)
{
}

bool FileModel::Initialize(ID3D11Device* device)
{
	if(!Model::LoadModelFromFile(this->m_ModelPath) || !Model::LoadTexture(device, this->m_TexturePath))
	{
		return false;
	}
	return Model::Initialize(device);
}
