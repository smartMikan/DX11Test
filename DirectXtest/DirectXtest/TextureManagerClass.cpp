////////////////////////////////////////////////////////////////////////////////
// Filename: texturemanagerclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "texturemanagerclass.h"


TextureManagerClass::TextureManagerClass()
{
	m_TextureArray = 0;
}


TextureManagerClass::TextureManagerClass(const TextureManagerClass& other)
{
}


TextureManagerClass::~TextureManagerClass()
{
}


bool TextureManagerClass::Initialize(int count, ID3D11Device* device)
{
	m_textureCount = count;

	// Create the color texture object.
	m_TextureArray = new TextureClass[m_textureCount];
	if (!m_TextureArray)
	{
		return false;
	}
	DirectX::CreateWICTextureFromFile(device, L"./Resources/tone_.png", nullptr, toneTexture.GetAddressOf());

	return true;
}


void TextureManagerClass::Shutdown()
{
	int i;


	// Release the texture objects.
	if (m_TextureArray)
	{
		for (i = 0; i<m_textureCount; i++)
		{
			m_TextureArray[i].Shutdown();
		}
		delete[] m_TextureArray;
		m_TextureArray = 0;
	}

	return;
}


bool TextureManagerClass::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const WCHAR* filename, int location)
{
	bool result;


	// Initialize the color texture object.
	result = m_TextureArray[location].Initialize(device, deviceContext, filename);
	if (!result)
	{
		return false;
	}

	return true;
}




ID3D11ShaderResourceView *TextureManagerClass::GetTexture(int id)
{
	return m_TextureArray[id].GetTexture();
}

ID3D11ShaderResourceView * const * TextureManagerClass::GetToonTexture()
{
	return toneTexture.GetAddressOf();
}
