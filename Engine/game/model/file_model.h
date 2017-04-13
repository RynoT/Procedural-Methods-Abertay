#pragma once

#include "model.h"

class FileModel : public Model
{
public:
	FileModel(char *modelPath, WCHAR *texturePath);

	virtual bool Initialize(ID3D11Device* device) override;

	inline const char* GetModelPath() const { return this->m_ModelPath; }

	inline const WCHAR* GetTexturePath() const { return this->m_TexturePath; }

private:
	char *m_ModelPath;
	WCHAR *m_TexturePath;
};
