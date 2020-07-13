﻿#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: userinterfaceclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _USERINTERFACECLASS_H_
#define _USERINTERFACECLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "textclass.h"
#include "minimapclass.h"
//#include "debugwindowclass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: UserInterfaceClass
////////////////////////////////////////////////////////////////////////////////
class UserInterfaceClass
{
public:
	UserInterfaceClass();
	UserInterfaceClass(const UserInterfaceClass& other);
	~UserInterfaceClass();

	bool Initialize(D3DClass* Direct3D, int screenHeight, int screenWidth);
	bool Initialize(D3DClass* Direct3D, int screenHeight, int screenWidth,ID3D11ShaderResourceView* renderTexture);
	void Shutdown();

	bool Frame(ID3D11DeviceContext* deviceContext, int fps,int cpu, float posX, float posY, float posZ,
		float rotX, float rotY, float rotZ);
	bool Render(D3DClass* Direct3D, ShaderManagerClass* ShaderManager, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
		XMMATRIX orthoMatrix);

	bool UpdateRenderCounts(ID3D11DeviceContext* deviceContext, int renderCount, int nodesDrawn, int nodesCulled);

private:
	bool UpdateFpsString(ID3D11DeviceContext* deviceContext, int fps);
	bool UpdateCpuString(ID3D11DeviceContext* deviceContext, int cpu);
	bool UpdatePositionStrings(ID3D11DeviceContext* deviceContext, float posX, float posY, float posZ,
		float rotX, float rotY, float rotZ);

private:
	FontClass * m_Font1;
	TextClass *m_FpsString,*m_CpuString , *m_VideoStrings, *m_PositionStrings;
	int m_previousFps;
	int m_previousPosition[6];
	TextClass* m_RenderCountStrings;
	MiniMapClass* m_MiniMap;
	//DebugWindowClass* m_DebugWindow;
};

#endif