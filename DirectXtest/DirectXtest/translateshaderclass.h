#pragma once

#include<d3d11.h>
#include<d3dcompiler.h>
#include<directxmath.h>
#include<fstream>

using namespace DirectX;
using namespace std;



class TranslateShaderClass
{
protected:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct TranslateBufferType
	{
		float translation;
		XMFLOAT3 padding;
	};

public:
	TranslateShaderClass();
	TranslateShaderClass(const TranslateShaderClass&);
	~TranslateShaderClass();

	virtual bool Initialize(ID3D11Device*, HWND);
	virtual void Shutdown();
	virtual bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*,float translation);

protected:
	virtual bool InitializeShader(ID3D11Device*, HWND, const WCHAR*, const WCHAR*);
	virtual void ShutdownShader();
	virtual void OutputShaderErrorMessage(ID3D10Blob*, HWND , const WCHAR* );
	
	virtual bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*,float translation);
	virtual void RenderShader(ID3D11DeviceContext*, int);

protected:
	ID3D11VertexShader * m_vertexShader;
	ID3D11PixelShader * m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;

	ID3D11SamplerState * m_sampleState;
	ID3D11Buffer* m_translateBuffer;

};

