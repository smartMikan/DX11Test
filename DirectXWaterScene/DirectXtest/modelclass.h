﻿#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <directxmath.h>
//#include "textureclass.h"
#include "ddstextureclass.h"
#include <fstream>
//#include "texturearrayclass.h"

using namespace std;
using namespace DirectX;



////////////////////////////////////////////////////////////////////////////////
// Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////
class ModelClass
{
private:
	//Here is the definition of our vertex type that will be used with the vertex buffer in this ModelClass.Also take note that this typedef must match the layout in the ColorShaderClass that will be looked at later in the project.

	//struct VertexType
	//{
	//	XMFLOAT3 position;
	//	XMFLOAT4 color;
	//};

	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
		XMFLOAT3 tangent;
		XMFLOAT3 binormal;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
		//float tx, ty, tz;
		//float bx, by, bz;
	};

	//The following two structures will be used for calculating the tangent and binormal.
	struct TempVertexType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	struct VectorType
	{
		float x, y, z;
	};


public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();
	//The functions here handle initializing and shutdown of the model's vertex and index buffers. The Render function puts the model geometry on the video card to prepare it for drawing by the color shader.

	//bool Initialize(ID3D11Device*, const WCHAR * modelFilename, const WCHAR * textureFilename1, const WCHAR* textureFilename2, const WCHAR * textureFilename3);
	bool Initialize(ID3D11Device*, const WCHAR * modelFilename, const WCHAR * textureFilename1);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();
	//ID3D11ShaderResourceView** GetTextureArray();

	//void CalculateModelVectors();
	//void CalculateTangentBinormal(TempVertexType vertex1, TempVertexType vertex2, TempVertexType vertex3, VectorType& tangent, VectorType& binormal);
	//void CalculateNormal(VectorType tangent, VectorType binormal, VectorType& normal);

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
	//The private variables in the ModelClass are the vertex and index buffer as well as two integers to keep track of the size of each buffer.Note that all DirectX 11 buffers generally use the generic ID3D11Buffer type and are more clearly identified by a buffer description when they are first created.

	bool LoadTexture(ID3D11Device*, const WCHAR * filename);
	//bool LoadTextures(ID3D11Device*, const WCHAR* textureFilename1, const WCHAR* textureFilename2, const WCHAR * filename3);
	void ReleaseTexture();
	//void ReleaseTextures();

	bool LoadModel(const WCHAR * filename);
	void ReleaseModel();

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	DDSTextureClass* m_Texture;
	ModelType* m_model;
	//TextureArrayClass* m_TextureArray;
};

#endif
