﻿#pragma once



#pragma comment(lib, "d3d11.lib")  //The first library contains all the Direct3D functionality for setting upand drawing 3D graphics in DirectX 11. 
#pragma comment(lib, "dxgi.lib")  //The second library contains tools to interface with the hardware on the computer to obtain information about the refresh rate of the monitor, the video card being used, and so forth.
#pragma comment(lib, "d3dcompiler.lib")  //The third library contains functionality for compiling shaders which we will cover in the next project.



#include <d3d11.h>
#include <directxmath.h>
#include <directxcolors.h>
#include <assert.h>
using namespace DirectX;


class D3DClass
{
public:
	D3DClass();
	D3DClass(const D3DClass&);
	~D3DClass();

	bool Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear);
	void Shutdown();

	void BeginScene(float red, float green, float blue, float alpha);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void GetProjectionMatrix(XMMATRIX&);
	void GetWorldMatrix(XMMATRIX&);
	void GetOrthoMatrix(XMMATRIX&);

	void GetVideoCardInfo(char*, int&);

	void TurnZBufferOn();
	void TurnZBufferOff();

	void TurnDepthStencilStateLessEqual();

	void TurnOnCulling();
	void TurnOffCulling();
	void EnableAlphaToCoverageBlending();
	void EnableWireframe();
	void DisableWireframe();

	void TurnOnAlphaBlending();
	void TurnOffAlphaBlending();

	void TurnOnParticleBlending();
	void TurnOffParticleBlending();

	ID3D11DepthStencilView* GetDepthStencilView();
	void SetBackBufferRenderTarget();

	void ResetViewport();

private:
	bool GetRefreshrate(int screenWidth, int screenHeight,unsigned int& numerator, unsigned int& denominator);

private:
	bool m_vsync_enabled;
	int m_videoCardMemory;
	char m_videoCardDescription[128];
	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11Texture2D* m_depthStencilBuffer;

	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilState* m_depthStencilStateLessEqual;
	ID3D11DepthStencilState* m_depthDisabledStencilState;

	ID3D11DepthStencilView* m_depthStencilView;

	ID3D11RasterizerState* m_rasterState;
	ID3D11RasterizerState* m_rasterStateNoCulling;
	ID3D11RasterizerState* m_rasterStateWireframe;

	XMMATRIX m_projectionMatrix;
	XMMATRIX m_worldMatrix;
	XMMATRIX m_orthoMatrix;



	ID3D11BlendState* m_alphaEnableBlendingState;
	ID3D11BlendState* m_alphaDisableBlendingState;
	ID3D11BlendState* m_alphaEnableBlendingState2;
	ID3D11BlendState* m_alphaEnableBlendingStateParticle;

	D3D11_VIEWPORT m_viewport;
};


//For those familiar with Direct3D already you may notice I don't have a view matrix variable in this class. 
//The reason being is that I will be putting it in a camera class that we will be looking at in future projects.




/*RefreshRateを取得------------->   CreateDXGIFactory
											|
									factory->EnumAdapters
											|
									adapter->EnumOutputs
											|
									adapterOutput->GetDisplayModeList => (&numModes)
											|
									displayModeList = new DXGI_MODE_DESC[numModes];
											| check
									adapterOutput->GetDisplayModeList <= (&numModes,displayModeList)
											|
									for(numModes){
									 if(list[i].Width==ScreenWidth&&Height==ScreenHeight)
									 {
										[一定時間refresh数]	     numerator = displayModeList[i].RefreshRate.Numerator;
										[時間値]	     　　　　denominator = displayModeList[i].RefreshRate.Denominator;
									 }
											|
							RefreshRate=numerator/denominator
											|
									delete[] displayModeList;
											|
									adapter->Release();
											|
									factory->Release();
											|
　スワップチェインの作成------------>swapChainDescの初期化、色々な数値設定
								　　　D3dclass.cpp 171行目から237行まで
											 |
											 |
									機能レベルの設定
									featureLevel = D3D_FEATURE_LEVEL_11_0;
											 |
											 |
									D3D11CreateDeviceAndSwapChain
											 |
  レンダリングターゲットの作成----->   m_swapChain->GetBuffer
												|
									  m_device->CreateRenderTargetView
												|
										backBufferPtr->Release()
										backBufferPtr = 0;
													|
  深度ステンシルテクスチャーの作成---> depthBufferDescの初期化、色々な数値設定
										D3dclass.cpp 324行目から334行まで
													|
										m_device->CreateTexture2D
													|
									   depthStencilDescの初期化、色々な数値設定
										D3dclass.cpp 354行目から375行まで
													|
										m_device->CreateDepthStencilState
													|
										m_deviceContext->OMSetDepthStencilState
													|
										depthStencilViewDescの初期化、色々な数値設定
										D3dclass.cpp 397行目から402行まで
													|
										m_device->CreateDepthStencilView
													|
  レンダリングターゲット設定------>		m_deviceContext->OMSetRenderTargets
													|
  ラスタライザの設定-------------->     rasterDescの色々な数値設定
										D3dclass.cpp 426行目から435行まで
													|
										m_device->CreateRasterizerState
													|
										m_deviceContext->RSSetState
　													|
 ビューポートの設定-------------->      viewportの色々な数値設定
										D3dclass.cpp 451行目から456行まで
													|
										m_deviceContext->RSSetViewports
													|
 XMMatrix取得----------------------->  m_projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);
									   m_worldMatrix = XMMatrixIdentity();
									   m_orthoMatrix = XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, screenNear, screenDepth);


*/