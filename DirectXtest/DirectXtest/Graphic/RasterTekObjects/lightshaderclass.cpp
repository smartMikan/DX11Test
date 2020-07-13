﻿////////////////////////////////////////////////////////////////////////////////
// Filename: lightshaderclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "lightshaderclass.h"


LightShaderClass::LightShaderClass()
{
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_sampleState = 0;
	m_matrixBuffer = 0;
	
	m_cameraBuffer = 0;

	//Set the light constant buffer to null in the class constructor.
	m_lightBuffer = 0;
}


LightShaderClass::LightShaderClass(const LightShaderClass& other)
{
}


LightShaderClass::~LightShaderClass()
{
}


bool LightShaderClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext,HWND hwnd)
{
	bool result;
	//The new light.vs and light.ps HLSL shader files are used as input to initialize the light shader.

	// Initialize the vertex and pixel shaders.
	
	result = InitializeShader(device, deviceContext, hwnd, L"./Shader/SpecMapVertexShader.hlsl", L"./Shader/SpecMapPixelShader.hlsl");
	if (!result)
	{
		return false;
	}

	return true;
}


void LightShaderClass::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	ShutdownShader();

	return;
}

//The Render function now takes in the light direction and light diffuse color as inputs.
//These variables are then sent into the SetShaderParameters function and finally set inside the shader itself.

bool LightShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, ID3D11ShaderResourceView ** textureArray, MatrixBufferType wvpMatrixBuffer, CameraBufferType cameraBuffer, LightBufferType lightBuffer)
{
	bool result;


	// Set the shader parameters that it will use for rendering.
	result = SetShaderParameters(deviceContext, wvpMatrixBuffer, textureArray, cameraBuffer, lightBuffer);
	if (!result)
	{
		return false;
	}

	// Now render the prepared buffers with the shader.
	RenderShader(deviceContext, indexCount);

	return true;
}

bool LightShaderClass::Render(ID3D11DeviceContext *deviceContext, int indexCount, ID3D11ShaderResourceView ** textureArray, XMMATRIX world, XMMATRIX view, XMMATRIX projection, XMFLOAT3 cameraPosition, XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor, XMFLOAT3 lightDirection, float specularPower, XMFLOAT4 specularColor)
{
	bool result;
	result = SetShaderParameters(deviceContext, GenarateMatrixBuffer(world, view, projection), textureArray, GenerateCameraBuffer(cameraPosition), GenerateLightBuffer(ambientColor, diffuseColor, lightDirection, specularPower, specularColor));
	if (!result)
	{
		return false;
	}
	// Now render the prepared buffers with the shader.
	RenderShader(deviceContext, indexCount);
	return false;
}

LightShaderClass::MatrixBufferType LightShaderClass::GenarateMatrixBuffer(XMMATRIX world, XMMATRIX view, XMMATRIX projection)
{
	MatrixBufferType temp;
	temp.world = world;
	temp.view = view;
	temp.projection = projection;
	return temp;
}

LightShaderClass::CameraBufferType LightShaderClass::GenerateCameraBuffer(XMFLOAT3 cameraPosition)
{
	CameraBufferType temp;
	temp.cameraPosition = cameraPosition;
	temp.padding = 0.0f;
	return temp;
}

LightShaderClass::LightBufferType LightShaderClass::GenerateLightBuffer(XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor, XMFLOAT3 lightDirection, float specularPower, XMFLOAT4 specularColor)
{
	LightBufferType temp;
	temp.ambientColor = ambientColor;
	temp.diffuseColor = diffuseColor;
	temp.lightDirection = lightDirection;
	temp.specularColor = specularColor;
	temp.specularPower = specularPower;
	return temp;
}


bool LightShaderClass::InitializeShader(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, const WCHAR* vsFilename, const WCHAR* psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	
	//The polygonLayout variable has been changed to have three elements instead of two.This is so that it can accommodate a normal vector in the layout.

	D3D11_INPUT_ELEMENT_DESC polygonLayout[5];
	unsigned int numElements;
	D3D11_SAMPLER_DESC samplerDesc;
	//D3D11_BUFFER_DESC matrixBufferDesc;

	//D3D11_BUFFER_DESC cameraBufferDesc;

	//We also add a new description variable for the light constant buffer.

	//D3D11_BUFFER_DESC lightBufferDesc;


	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;
	
	//Load in the new light vertex shader.

	// Compile the vertex shader code.
	result = D3DCompileFromFile(vsFilename, NULL, NULL, "main", "vs_5_0", D3D11_SHADER_MAJOR_VERSION, 0,
		&vertexShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBoxW(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}
	
	//Load in the new light pixel shader.

	// Compile the pixel shader code.
	result = D3DCompileFromFile(psFilename, NULL, NULL, "main", "ps_5_0", D3D11_SHADER_MAJOR_VERSION, 0,
			&pixelShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
		}
		// If there was nothing in the error message then it simply could not find the file itself.
		else
		{
			MessageBoxW(hwnd, psFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// Create the vertex shader from the buffer.
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result))
	{
		return false;
	}

	// Create the pixel shader from the buffer.
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result))
	{
		return false;
	}

	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;
	
	//One of the major changes to the shader initialization is here in the polygonLayout.We add a third element for the normal vector that will be used for lighting.The semantic name is NORMAL and the format is the regular DXGI_FORMAT_R32G32B32_FLOAT which handles 3 floats for the x, y, and z of the normal vector.The layout will now match the expected input to the HLSL vertex shader.

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	//The layout now includes a tangent and binormal element which are setup the same as the normal element with the exception of the semantic name.
	polygonLayout[3].SemanticName = "TANGENT";
	polygonLayout[3].SemanticIndex = 0;
	polygonLayout[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[3].InputSlot = 0;
	polygonLayout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[3].InstanceDataStepRate = 0;

	polygonLayout[4].SemanticName = "BINORMAL";
	polygonLayout[4].SemanticIndex = 0;
	polygonLayout[4].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[4].InputSlot = 0;
	polygonLayout[4].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[4].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(),
		&m_layout);
	if (FAILED(result))
	{
		return false;
	}

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	result = device->CreateSamplerState(&samplerDesc, &m_sampleState);
	if (FAILED(result))
	{
		return false;
	}


	m_matrixBuffer = new ConstantBuffer<MatrixBufferType>();

	result = m_matrixBuffer->Initialize(device,deviceContext);
	if (FAILED(result))
	{
		return false;
	}

	//// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	//matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	//matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	//matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//matrixBufferDesc.MiscFlags = 0;
	//matrixBufferDesc.StructureByteStride = 0;

	//// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	//result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	//if (FAILED(result))
	//{
	//	return false;
	//}
	
	m_cameraBuffer = new ConstantBuffer<CameraBufferType>();

	result = m_cameraBuffer->Initialize(device,deviceContext);
	if (FAILED(result))
	{
		return false;
	}
	//// Setup the description of the camera dynamic constant buffer that is in the vertex shader.
	//cameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	//cameraBufferDesc.ByteWidth = sizeof(CameraBufferType);
	//cameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//cameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//cameraBufferDesc.MiscFlags = 0;
	//cameraBufferDesc.StructureByteStride = 0;

	//// Create the camera constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	//result = device->CreateBuffer(&cameraBufferDesc, NULL, &m_cameraBuffer);
	//if (FAILED(result))
	//{
	//	return false;
	//}


	m_lightBuffer = new ConstantBuffer<LightBufferType>();

	result = m_lightBuffer->Initialize(device,deviceContext);
	if (FAILED(result))
	{
		return false;
	}

	////Here we setup the light constant buffer description which will handle the diffuse light color and light direction.Pay attention to the size of the constant buffers, if they are not multiples of 16 you need to pad extra space on to the end of them or the CreateBuffer function will fail.In this case the constant buffer is 28 bytes with 4 bytes padding to make it 32.

	//// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	//// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
	//lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	//lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	//lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//lightBufferDesc.MiscFlags = 0;
	//lightBufferDesc.StructureByteStride = 0;

	//// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	//result = device->CreateBuffer(&lightBufferDesc, NULL, &m_lightBuffer);
	//if (FAILED(result))
	//{
	//	return false;
	//}

	return true;
}


void LightShaderClass::ShutdownShader()
{

	// Release the sampler state.
	if (m_sampleState)
	{
		m_sampleState->Release();
		m_sampleState = 0;
	}

	// Release the layout.
	if (m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	// Release the pixel shader.
	if (m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = 0;
	}

	// Release the vertex shader.
	if (m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = 0;
	}

	return;
}


void LightShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, const WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long long bufferSize, i;
	ofstream fout;


	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for (i = 0; i < bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBoxW(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}
//The SetShaderParameters function now takes in lightDirection and diffuseColor as inputs.

bool LightShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext,
	MatrixBufferType wvpMatrixBuffer, ID3D11ShaderResourceView ** textureArray, 
	CameraBufferType cameraBuffer, LightBufferType lightBuffer)
{
	HRESULT result;
	//D3D11_MAPPED_SUBRESOURCE mappedResource;
	unsigned int bufferNumber;
	//MatrixBufferType* dataPtr;
	//LightBufferType* dataPtr2;
	//CameraBufferType* dataPtr3;

	// Transpose the matrices to prepare them for the shader.
	wvpMatrixBuffer.world = XMMatrixTranspose(wvpMatrixBuffer.world);
	wvpMatrixBuffer.view = XMMatrixTranspose(wvpMatrixBuffer.view);
	wvpMatrixBuffer.projection = XMMatrixTranspose(wvpMatrixBuffer.projection);
	/*worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);*/

	//deviceContext->UpdateSubresource(m_matrixBuffer, 0, nullptr, &wvpMatrixBuffer, sizeof(XMMATRIX),0);

	//// Lock the constant buffer so it can be written to.
	//result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	//if (FAILED(result))
	//{
	//	return false;
	//}

	//// Get a pointer to the data in the constant buffer.
	//dataPtr = (MatrixBufferType*)mappedResource.pData;

	//// Copy the matrices into the constant buffer.
	//dataPtr->world = wvpMatrixBuffer.world;
	//dataPtr->view = wvpMatrixBuffer.view;
	//dataPtr->projection = wvpMatrixBuffer.projection;

	//// Unlock the constant buffer.
	//deviceContext->Unmap(m_matrixBuffer, 0);

	m_matrixBuffer->data.world = wvpMatrixBuffer.world;
	m_matrixBuffer->data.view = wvpMatrixBuffer.view;
	m_matrixBuffer->data.projection = wvpMatrixBuffer.projection;

    result = m_matrixBuffer->ApplyChanges();
	if (FAILED(result))
	{
		return false;
	}

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Now set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, m_matrixBuffer->GetAddress());

	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 3, textureArray);
	


	////Here we lock the camera buffer and set the camera position value in it.
	//// Lock the camera constant buffer so it can be written to.
	//result = deviceContext->Map(m_cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	//if (FAILED(result))
	//{
	//	return false;
	//}

	//// Get a pointer to the data in the constant buffer.
	//dataPtr3 = (CameraBufferType*)mappedResource.pData;

	//// Copy the camera position into the constant buffer.
	//dataPtr3->cameraPosition = cameraBuffer.cameraPosition;
	//dataPtr3->padding = 0.0f;

	//// Unlock the camera constant buffer.
	//deviceContext->Unmap(m_cameraBuffer, 0);


	m_cameraBuffer->data.cameraPosition = cameraBuffer.cameraPosition;
	m_cameraBuffer->data.padding = 0.0f;

	result = m_cameraBuffer->ApplyChanges();
	if (FAILED(result))
	{
		return false;
	}


	//Note that we set the bufferNumber to 1 instead of 0 before setting the constant buffer. 
	//This is because it is the second buffer in the vertex shader (the first being the matrix buffer).

	// Set the position of the camera constant buffer in the vertex shader.
	bufferNumber = 1;

	// Now set the camera constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, m_cameraBuffer->GetAddress());

	////The light constant buffer is setup the same way as the matrix constant buffer.
	////We first lock the buffer and get a pointer to it.
	////After that we set the diffuse color and light direction using that pointer.
	////Once the data is set we unlock the buffer and then set it in the pixel shader.
	////Note that we use the PSSetConstantBuffers function instead of VSSetConstantBuffers since this is a pixel shader buffer we are setting.

	//// Lock the light constant buffer so it can be written to.
	//result = deviceContext->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	//if (FAILED(result))
	//{
	//	return false;
	//}

	//// Get a pointer to the data in the constant buffer.
	//dataPtr2 = (LightBufferType*)mappedResource.pData;

	//// Copy the lighting variables into the constant buffer.
	//dataPtr2->ambientColor = lightBuffer.ambientColor;
	//dataPtr2->diffuseColor = lightBuffer.diffuseColor;
	//dataPtr2->lightDirection = lightBuffer.lightDirection;
	////dataPtr2->padding = 0.0f;
	//dataPtr2->specularColor = lightBuffer.specularColor;
	//dataPtr2->specularPower = lightBuffer.specularPower;

	//// Unlock the constant buffer.
	//deviceContext->Unmap(m_lightBuffer, 0);


	m_lightBuffer->data.ambientColor = lightBuffer.ambientColor;
	m_lightBuffer->data.diffuseColor = lightBuffer.diffuseColor;
	m_lightBuffer->data.lightDirection = lightBuffer.lightDirection;
	//m_lightBuffer->data.padding = 0.0f;
	m_lightBuffer->data.specularColor = lightBuffer.specularColor;
	m_lightBuffer->data.specularPower = lightBuffer.specularPower;

	result = m_lightBuffer->ApplyChanges();
	if (FAILED(result))
	{
		return false;
	}

	// Set the position of the light constant buffer in the pixel shader.
	bufferNumber = 0;

	// Finally set the light constant buffer in the pixel shader with the updated values.
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, m_lightBuffer->GetAddress());


	return true;
}


void LightShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{

	// Set the vertex input layout.
	deviceContext->IASetInputLayout(m_layout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	// Set the sampler state in the pixel shader.
	deviceContext->PSSetSamplers(0, 1, &m_sampleState);

	// Render the triangle.
	deviceContext->DrawIndexed(indexCount, 0, 0);

	return;
}