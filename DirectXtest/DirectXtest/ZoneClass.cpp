////////////////////////////////////////////////////////////////////////////////
// Filename: zoneclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "zoneclass.h"

ZoneClass::ZoneClass()
{
	m_UserInterface = 0;
	m_Camera = 0;
	m_Position = 0;
	m_Light = 0;
	m_Terrain = 0;
	m_SkyDome = 0;
	m_DayLightSkyCube = 0;
	m_SunsetSkyCube = 0;
	m_DesertSkyCube = 0;
	m_NebulaSkyCube = 0;
	m_PlanetSkyCube = 0;
	m_Model = 0;
	m_MeshModel = 0;
	m_Frustum = 0;
	m_ParticleSystem = 0;

	m_RenderTexture = 0;

}


ZoneClass::ZoneClass(const ZoneClass& other)
{
}


ZoneClass::~ZoneClass()
{
}
bool ZoneClass::Initialize(D3DClass* Direct3D, HWND hwnd, int screenWidth, int screenHeight, float screenDepth)
{
	bool result;

	this->device = Direct3D->GetDevice();
	this->deviceContext = Direct3D->GetDeviceContext();
	InitializeShaders();
	// Create the user interface object.
	m_UserInterface = new UserInterfaceClass;
	if (!m_UserInterface)
	{
		return false;
	}

	// Initialize the user interface object.
	result = m_UserInterface->Initialize(Direct3D, screenHeight, screenWidth);
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the user interface object.", L"Error", MB_OK);
		return false;
	}

	// Set the UI to display by default.
	m_displayUI = true;

	// Create the camera object.
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera and build the matrices needed for rendering.
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	m_Camera->Render();
	m_Camera->RenderBaseViewMatrix();

	// Create the position object.
	m_Position = new PositionClass;
	if (!m_Position)
	{
		return false;
	}

	// Set the initial position and rotation.
	m_Position->SetPosition(10.0f, 30.0f,10.0f);
	m_Position->SetRotation(0.0f, 0.0f, 0.0f);


	// Create the frustum object.
	m_Frustum = new FrustumClass;
	if (!m_Frustum)
	{
		return false;
	}

	// Initialize the frustum object.
	m_Frustum->Initialize(screenDepth);

	// Create the sky dome object.
	m_SkyDome = new SkyDomeClass;
	if (!m_SkyDome)
	{
		return false;
	}

	// Initialize the sky dome object.
	result = m_SkyDome->Initialize(Direct3D->GetDevice());
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the sky dome object.", L"Error", MB_OK);
		return false;
	}

	// Create the sky dome object.
	m_DayLightSkyCube = new SkyCubeClass;
	if (!m_DayLightSkyCube)
	{
		return false;
	}

	// Initialize the sky dome object.
	result = m_DayLightSkyCube->Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext() ,L"./3DModel/Texture/daylight.jpg");
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the sky cube object.", L"Error", MB_OK);
		return false;
	}


	m_SunsetSkyCube = new SkyCubeClass;
	if (!m_SunsetSkyCube)
	{
		return false;
	}

	// Initialize the sky dome object.
	result = m_SunsetSkyCube->Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), std::vector<std::wstring>{
			L"./3DModel/Texture/sunset_posX.bmp", L"./3DModel/Texture/sunset_negX.bmp",
			L"./3DModel/Texture/sunset_posY.bmp", L"./3DModel/Texture/sunset_negY.bmp",
			L"./3DModel/Texture/sunset_posZ.bmp", L"./3DModel/Texture/sunset_negZ.bmp", });
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the sunset sky cube object.", L"Error", MB_OK);
		return false;
	}

	m_DesertSkyCube = new SkyCubeClass;
	if (!m_DesertSkyCube)
	{
		return false;
	}

	// Initialize the sky dome object.
	result = m_DesertSkyCube->Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), L"./3DModel/Texture/desertcube1024.dds");
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the desert sky cube object.", L"Error", MB_OK);
		return false;
	}

	m_NebulaSkyCube = new SkyCubeClass;
	if (!m_NebulaSkyCube)
	{
		return false;
	}
	// Initialize the sky dome object.
	result = m_NebulaSkyCube->Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), std::vector<std::wstring>{
		    L"./SkyBoxVolume2/Stars01/rightImage.png", L"./SkyBoxVolume2/Stars01/leftImage.png",
			L"./SkyBoxVolume2/Stars01/upImage.png", L"./SkyBoxVolume2/Stars01/downImage.png",
			L"./SkyBoxVolume2/Stars01/backImage.png", L"./SkyBoxVolume2/Stars01/frontImage.png", });
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the sunset sky cube object.", L"Error", MB_OK);
		return false;
	}

	m_PlanetSkyCube = new SkyCubeClass;
	if (!m_PlanetSkyCube)
	{
		return false;
	}
	// Initialize the sky dome object.
	result = m_PlanetSkyCube->Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), std::vector<std::wstring>{
		L"./SkyBoxVolume2/DeepSpaceBlueWithPlanet/leftImage.png", L"./SkyBoxVolume2/DeepSpaceBlueWithPlanet/rightImage.png",
			L"./SkyBoxVolume2/DeepSpaceBlueWithPlanet/upImage.png", L"./SkyBoxVolume2/DeepSpaceBlueWithPlanet/downImage.png",
			L"./SkyBoxVolume2/DeepSpaceBlueWithPlanet/frontImage.png", L"./SkyBoxVolume2/DeepSpaceBlueWithPlanet/backImage.png", });
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the sunset sky cube object.", L"Error", MB_OK);
		return false;
	}


	// Create the terrain object.
	m_Terrain = new TerrainClass;
	if (!m_Terrain)
	{
		return false;
	}

	// Initialize the terrain object.
	result = m_Terrain->Initialize(Direct3D->GetDevice(), ".//Terrain/Setup.txt");
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the terrain object.", L"Error", MB_OK);
		return false;
	}

	// Create the model object.
	m_Model = new ModelClass;
	if (!m_Model)
	{
		return false;
	}

	// Initialize the model object.
	result = m_Model->Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), L"./3DModel/Cube.txt", L"./3DModel/Texture/stone01.dds", L"./3DModel/Texture/bump01.dds", L"./3DModel/Texture/light01.dds");

	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_MeshModel = new GameObjectClass;
	if (!m_MeshModel)
	{
		return false;
	}
	//Initialize the model object.
	result = m_MeshModel->Initialize("./3DModel/Hip_Hop_Dancing.fbx", Direct3D->GetDevice(), Direct3D->GetDeviceContext(), cb_vs_wvpBuffer, cb_ps_material, d3dvertexshader_animation.get());
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the mesh model object.", L"Error", MB_OK);
		return false;
	}

	m_MeshModel->InitAnimation(cb_bones);
	m_MeshModel->AddAnimation("./3DModel/Bellydancing.fbx", cb_bones);
	m_MeshModel->AddAnimation("./3DModel/Boxing.fbx", cb_bones);
	m_MeshModel->AddAnimation("./3DModel/sheathsword1.fbx", cb_bones);

	// Create the light object.
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	
	//m_Light->SetDirection(-1.0f, -1.0f, 1.0f);
	
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);

	m_Light->GenerateOrthoMatrix(20.0f, SHADOWMAP_DEPTH, SHADOWMAP_NEAR);

	m_Light->SetPosition(9.0, 8.0f, -0.1f);
	m_Light->SetLookAt(-9.0, 0.0f, 0.0f);

	// Create the render to texture object.
	m_RenderTexture = new RenderTextureClass;
	if (!m_RenderTexture)
	{
		return false;
	}

	// Initialize the render to texture object.
	result = m_RenderTexture->Initialize(Direct3D->GetDevice(), SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, SHADOWMAP_DEPTH, SHADOWMAP_NEAR);
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the render to texture object.", L"Error", MB_OK);
		return false;
	}


	// Create the particle system object.
	m_ParticleSystem = new ParticleSystemClass;
	if (!m_ParticleSystem)
	{
		return false;
	}

	// Initialize the particle system object.
	result = m_ParticleSystem->Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), L"./Resources/star2.tga");
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the particle object.", L"Error", MB_OK);
		return false;
	}


	// Set wire frame rendering initially to disabled.
	m_wireFrame = false;

	// Set the rendering of cell lines initially to disabled.
	m_cellLines = false;

	// Set the user locked to the terrain height for movement.
	m_heightLocked = true;

	//Set the cubemap sky to to disabled.
	m_cubemapsky = false;


	//create SkinModelClass
	mCharacterModel = new SkinnedModelClass(Direct3D->GetDevice(), string("./3DModel/AnimationFile/soldier.m3d"), wstring(L"./3DModel/AnimationFile/"));

	//create SkinModel Instance
	mCharacterInstance1.Model = mCharacterModel;
	mCharacterInstance2.Model = mCharacterModel;
	mCharacterInstance1.TimePos = 0.0f;
	mCharacterInstance2.TimePos = 0.0f;
	mCharacterInstance1.ClipName = "Take1";
	mCharacterInstance2.ClipName = "Take1";
	mCharacterInstance1.FinalTransforms.resize(mCharacterModel->SkinnedData.BoneCount());
	mCharacterInstance2.FinalTransforms.resize(mCharacterModel->SkinnedData.BoneCount());

	mCharacterInstance1.position = new PositionClass();
	mCharacterInstance2.position = new PositionClass();

	XMMATRIX modelScale = XMMatrixScaling(0.05f, 0.05f, -0.05f);
	XMMATRIX modelRot = XMMatrixRotationY(0);
	XMMATRIX modelOffset = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	XMStoreFloat4x4(&mCharacterInstance1.World, modelScale * modelRot * modelOffset);

	XMStoreFloat4x4(&mCharacterInstance2.World, modelScale * modelRot * modelOffset);

	mCharacterInstance1.Update(0.01f);
	mCharacterInstance2.Update(0.01f);

	modelPosition = XMLoadFloat4x4(&mCharacterInstance1.World);
	mCharacterInstance1.position->SetPosition(0, 0, 0);
	mCharacterInstance2.position->SetPosition(10, 0, 15);


	HRESULT hr;
	//initialize constant buffer
	hr = this->cb_vs_wvpBuffer.Initialize(this->device.Get(), this->deviceContext.Get());
	COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");
	cb_vs_wvpBuffer.SetDebugName("cb_vs_world");

	hr = this->cb_ps_light.Initialize(this->device.Get(), this->deviceContext.Get());
	COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");
	cb_ps_light.SetDebugName("cb_ps_light");


	hr = this->cb_ps_shadowMatrix.Initialize(this->device.Get(), this->deviceContext.Get());
	COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");


	hr = this->cb_ps_camera.Initialize(this->device.Get(), this->deviceContext.Get());
	COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");
	cb_ps_camera.SetDebugName("cb_ps_common");

	hr = this->cb_ps_material.Initialize(this->device.Get(), this->deviceContext.Get());
	COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");
	cb_ps_material.SetDebugName("ps_material");

	hr = this->cb_bones.Initialize(this->device.Get(), this->deviceContext.Get());
	COM_ERROR_IF_FAILED(hr, "Failed to initialize constant buffer.");
	cb_bones.SetDebugName("vs_bone_transforms");
	
	this->cb_ps_light.data.ambientLightColor = XMFLOAT3(1.0f, 1.0f, 1.0f);
	this->cb_ps_light.data.ambientLightStrength = 0.3f;


	

	return true;
}

void ZoneClass::Shutdown()
{
	if (mCharacterInstance1.position) {
		delete mCharacterInstance1.position;
		mCharacterInstance1.position = 0;
	}

	if (mCharacterInstance2.position) {
		delete mCharacterInstance2.position;
		mCharacterInstance2.position = 0;
	}

	if (mCharacterModel) {
		delete mCharacterModel;
		mCharacterModel = 0;
	}

	// Release the particle system object.
	if (m_ParticleSystem)
	{
		m_ParticleSystem->Shutdown();
		delete m_ParticleSystem;
		m_ParticleSystem = 0;
	}

	// Release the terrain object.
	if (m_Terrain)
	{
		m_Terrain->Shutdown();
		delete m_Terrain;
		m_Terrain = 0;
	}

	// Release the sky dome object.
	if (m_DayLightSkyCube)
	{
		m_DayLightSkyCube->Shutdown();
		delete m_DayLightSkyCube;
		m_DayLightSkyCube = 0;
	}
	if (m_SunsetSkyCube)
	{
		m_SunsetSkyCube->Shutdown();
		delete m_SunsetSkyCube;
		m_SunsetSkyCube = 0;
	}
	if (m_DesertSkyCube)
	{
		m_DesertSkyCube->Shutdown();
		delete m_DesertSkyCube;
		m_DesertSkyCube = 0;
	}

	if (m_NebulaSkyCube)
	{
		m_NebulaSkyCube->Shutdown();
		delete m_NebulaSkyCube;
		m_NebulaSkyCube = 0;
	}
	if (m_PlanetSkyCube)
	{
		m_PlanetSkyCube->Shutdown();
		delete m_PlanetSkyCube;
		m_PlanetSkyCube = 0;
	}

	// Release the sky dome object.
	if (m_SkyDome)
	{
		m_SkyDome->Shutdown();
		delete m_SkyDome;
		m_SkyDome = 0;
	}

	// Release the render to texture object.
	if (m_RenderTexture)
	{
		m_RenderTexture->Shutdown();
		delete m_RenderTexture;
		m_RenderTexture = 0;
	}

	// Release the frustum object.
	if (m_Frustum)
	{
		delete m_Frustum;
		m_Frustum = 0;
	}

	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the Meshmodel object.
	if (m_MeshModel) {
		m_MeshModel->Shutdown();
		delete m_MeshModel;
		m_MeshModel = 0;
	}

	// Release the model object.
	if (m_Model) {
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	// Release the position object.
	if (m_Position)
	{
		delete m_Position;
		m_Position = 0;
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the user interface object.
	if (m_UserInterface)
	{
		m_UserInterface->Shutdown();
		delete m_UserInterface;
		m_UserInterface = 0;
	}

	return;
}
bool ZoneClass::Frame(D3DClass* Direct3D, InputClass* Input, ShaderManagerClass* ShaderManager, TextureManagerClass* TextureManager, float frameTime, int fps, int cpu)
{
	

	bool result, foundHeight;
	float posX, posY, posZ, rotX, rotY, rotZ, height;



	// Do the frame input processing.
	HandleMovementInput(Input, frameTime,fps);

	// Get the view point position/rotation.
	mCharacterInstance1.position->GetPosition(posX, posY, posZ);

	//mCharacterInstance2.position->GetPosition(posX, posY, posZ);

	//m_Position->GetPosition(posX, posY, posZ);
	m_Position->GetRotation(rotX, rotY, rotZ);

	// Do the frame processing for the user interface.
	result = m_UserInterface->Frame(Direct3D->GetDeviceContext(), fps, cpu, posX, posY, posZ, rotX, rotY, rotZ);
	if (!result)
	{
		return false;
	}

	// Do the terrain frame processing.
	m_Terrain->Frame();

	// Run the frame processing for the particle system.
	m_ParticleSystem->Frame(frameTime, Direct3D->GetDeviceContext());

	// If the height is locked to the terrain then position the camera on top of it.
	if (m_heightLocked)
	{
		// Get the height of the triangle that is directly underneath the given camera position.
		foundHeight = m_Terrain->GetHeightAtPosition(posX, posZ, height);
		if (foundHeight)
		{
			// If there was a triangle under the camera then position the camera just above it by one meter.
			//m_Position->SetPosition(posX, height + 5.0f, posZ);
			mCharacterInstance1.position->SetPosition(posX, height, posZ);
			//m_Camera->SetPosition(posX, height, posZ);
		}
	}


	//m_Light->Frame();


	// Render the graphics.
	result = Render(Direct3D, ShaderManager, TextureManager);
	if (!result)
	{
		return false;
	}

	return true;
}
void ZoneClass::HandleMovementInput(InputClass* Input, float frameTime,float fps)
{
	bool keyDown;
	float posX, posY, posZ, rotX, rotY, rotZ;
	XMFLOAT3 orbitposition = mCharacterInstance1.position->GetPosition();
		/*XMFLOAT3(1, 1, 1);*/
	//XMVECTOR a = XMLoadFloat3(&orbitposition);
	//a = XMVector3TransformCoord(a, modelPosition);
	//XMStoreFloat3(&orbitposition, a);

	// Set the frame time for calculating the updated position.
	mCharacterInstance1.position->SetFrameTime(frameTime);
	m_Position->SetFrameTime(frameTime);
	m_Light->SetFrameTime(frameTime);

	float Deltatime = 1 / fps;

	//limit the range of delta time
	if (Deltatime > 1.0f)
	{
		Deltatime = 0.0f;
	}

	// Handle the input.
	keyDown = Input->IsLeftPressed() || Input->IsAPressed();
	//m_Position->TurnLeft(keyDown);
	mCharacterInstance1.position->TurnLeft(keyDown);
	m_Position->Orbit(keyDown,true,orbitposition);

	keyDown = Input->IsRightPressed() || Input->IsDPressed();
	//m_Position->TurnRight(keyDown);
	mCharacterInstance1.position->TurnRight(keyDown);
	m_Position->Orbit(keyDown, false, orbitposition);

	keyDown = Input->IsUpPressed() || Input->IsWPressed();
	//m_Position->MoveForward(keyDown);
	mCharacterInstance1.position->MoveForward(keyDown);

	if (keyDown) {
		//update the model animation
		mCharacterInstance1.Update(Deltatime);
	}

	keyDown = Input->IsDownPressed() || Input->IsSPressed();
	//m_Position->MoveBackward(keyDown);
	mCharacterInstance1.position->MoveBackward(keyDown);

	if (keyDown) {
		//update the model animation
		mCharacterInstance1.Update(-Deltatime);
	}


	keyDown = Input->IsSpacePressed();
	//m_Position->MoveUpward(keyDown);
	mCharacterInstance1.position ->MoveUpward(keyDown);

	keyDown = Input->IsLeftCtrlPressed();
	//m_Position->MoveDownward(keyDown);
	mCharacterInstance1.position->MoveDownward(keyDown);

	keyDown = Input->IsPgUpPressed() /*|| Input->GetVertical() < 0*/;
	m_Position->LookUpward(keyDown);

	keyDown = Input->IsPgDownPressed() /*|| Input->GetVertical() > 0*/;
	m_Position->LookDownward(keyDown);

	// Get the view point position/rotation.
	m_Position->GetPosition(posX, posY, posZ);
	m_Position->GetRotation(rotX, rotY, rotZ);

	mCharacterInstance1.position->SetRotation(0, rotY, 0);
	// Set the position of the camera.
	m_Camera->SetPosition(posX, posY, posZ);
	m_Camera->SetRotation(rotX, rotY, rotZ);

	//// Set the rotation of the light.
	//keyDown = Input->IsQPressed();
	//m_Light->TurnLeft(keyDown);

	//keyDown = Input->IsWPressed();
	//m_Light->TurnRight(keyDown);

	// Determine if the user interface should be displayed or not.
	if (Input->IsF1Toggled())
	{
		m_displayUI = !m_displayUI;
	}

	// Determine if the terrain should be rendered in wireframe or not.
	if (Input->IsF2Toggled())
	{
		m_wireFrame = !m_wireFrame;
	}
	
	// Determine if we should render the lines around each terrain cell.
	if (Input->IsF3Toggled())
	{
		m_cellLines = !m_cellLines;
	}

	// Determine if we should be locked to the terrain height when we move around or not.
	if (Input->IsF4Toggled())
	{
		m_heightLocked = !m_heightLocked;
	}

	/*if (Input->IsF5Toggled())
	{
		m_particleFollow = !m_particleFollow;
		if (m_particleFollow == true) {
			m_ParticleSystem->SetParticleProperty();
		}
		else
		{
			m_ParticleSystem->SetParticleProperty(500, 10, 500, -3, 1, 0.2, 1000);
		}
		
	}*/

	if (Input->IsF6Toggled())
	{
		m_cubemapsky = (m_cubemapsky + 1) % 6;
	}

	return;
}

bool ZoneClass::RenderSceneToTexture(D3DClass* Direct3D, ShaderManagerClass* ShaderManager)
{
	XMMATRIX worldMatrix, lightViewMatrix, lightOrthoMatrix;
	float posX, posY, posZ;
	bool result;


	// Set the render target to be the render to texture.
	m_RenderTexture->SetRenderTarget(Direct3D->GetDeviceContext(),Direct3D->GetDepthStencilView());

	// Clear the render to texture.
	m_RenderTexture->ClearRenderTarget(Direct3D->GetDeviceContext(),Direct3D->GetDepthStencilView(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the light view matrix based on the light's position.
	m_Light->GenerateViewMatrix();

	// Get the world matrix from the d3d object.
	Direct3D->GetWorldMatrix(worldMatrix);

	// Get the view and orthographic matrices from the light object.
	m_Light->GetViewMatrix(lightViewMatrix);
	m_Light->GetOrthoMatrix(lightOrthoMatrix);
	
	deviceContext->PSSetConstantBuffers(3, 1, cb_ps_shadowMatrix.GetAddress());

	cb_ps_shadowMatrix.data.shadowMatrix = XMMatrixTranspose(lightViewMatrix * lightOrthoMatrix);
	cb_ps_shadowMatrix.ApplyChanges();
	
	deviceContext->VSSetShader(d3dvertexshader_shadowmap_anim.get()->GetShader(device.Get()), NULL, 0);
	deviceContext->IASetInputLayout(d3dvertexshader_shadowmap_anim.get()->GetLayout());
	m_MeshModel->Draw(worldMatrix, lightViewMatrix, lightOrthoMatrix);


	

	//Direct3D->GetWorldMatrix(worldMatrix);

	//// Render the terrain cells (and cell lines if needed).
	//for (int i = 0; i < m_Terrain->GetCellCount(); i++)
	//{
	//	// Render each terrain cell if it is visible only.
	//	result = m_Terrain->RenderCell(Direct3D->GetDeviceContext(), i, m_Frustum);
	//	if (result)
	//	{
	//		// Render terrain cells with the depth shader.
	//		result = ShaderManager->RenderDepthShader(Direct3D->GetDeviceContext(), m_Terrain->GetCellIndexCount(i), worldMatrix, lightViewMatrix, lightOrthoMatrix);
	//		if (!result)
	//		{
	//			return false;
	//		}
	//	}
	//}

	// Reset the world matrix.

	// Setup the translation matrix for the cube model.

	modelPosition = worldMatrix;
	modelPosition = XMMatrixTranslation(cubeTranslation[0], cubeTranslation[1], cubeTranslation[2]);

	m_Model->Render(Direct3D->GetDeviceContext());
	result = ShaderManager->RenderDepthShader(Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), modelPosition, lightViewMatrix, lightOrthoMatrix);
	if (!result)
	{
		return false;
	}


	// Reset the world matrix.
	Direct3D->GetWorldMatrix(worldMatrix);
	XMMATRIX modelScale = XMMatrixScaling(0.05f, 0.05f, -0.05f);
	XMMATRIX modelRot = XMMatrixRotationY(mCharacterInstance1.position->GetRotationY() * 0.0174532925f);
	XMFLOAT3 position = mCharacterInstance1.position->GetPosition();
	XMMATRIX modelOffset = XMMatrixTranslation(position.x, position.y, position.z);
	//get model world matrix
	//modelPosition = XMLoadFloat4x4(&mCharacterInstance1.World);
	modelPosition = modelScale * modelRot * modelOffset;



	//render model for each subset
	for (UINT subset = 0; subset < mCharacterInstance1.Model->SubsetCount; ++subset)
	{

		ShaderManager->RenderDepthShader(Direct3D->GetDeviceContext(), modelPosition, lightViewMatrix, lightOrthoMatrix);

		//DrawCall
		mCharacterInstance1.Model->ModelMesh.Draw(Direct3D->GetDeviceContext(), subset);
	}



	position = mCharacterInstance2.position->GetPosition();
	modelOffset = XMMatrixTranslation(position.x, position.y, position.z);
	modelPosition = modelScale * modelOffset;

	//render model for each subset
	for (UINT subset = 0; subset < mCharacterInstance2.Model->SubsetCount; ++subset)
	{

		ShaderManager->RenderDepthShader(Direct3D->GetDeviceContext(), modelPosition, lightViewMatrix, lightOrthoMatrix);

		//DrawCall
		mCharacterInstance2.Model->ModelMesh.Draw(Direct3D->GetDeviceContext(), subset);
	}





	// Reset the render target back to the original back buffer and not the render to texture anymore.
	Direct3D->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	Direct3D->ResetViewport();

	return true;
}

bool ZoneClass::Render(D3DClass* Direct3D, ShaderManagerClass* ShaderManager, TextureManagerClass* TextureManager)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, baseViewMatrix, orthoMatrix;
	XMMATRIX lightViewMatrix, lightOrthoMatrix;
	bool result;
	XMFLOAT3 cameraPosition;
	
	int i;

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Camera->GetBaseViewMatrix(baseViewMatrix);
	Direct3D->GetOrthoMatrix(orthoMatrix);
	
	// Get the position of the camera.
	cameraPosition = m_Camera->GetPosition();
	// Construct the frustum.
	m_Frustum->ConstructFrustum(projectionMatrix, viewMatrix);

	result = RenderSceneToTexture(Direct3D, ShaderManager);
	if (!result)
	{
		return false;
	}
	// Clear the buffers to begin the scene.
	Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the light view matrix based on the light's position.
	m_Light->GenerateViewMatrix();

	// Get the light's view and projection matrices from the light object.
	m_Light->GetViewMatrix(lightViewMatrix);
	m_Light->GetOrthoMatrix(lightOrthoMatrix);


	// Turn off back face culling.
	Direct3D->TurnOffCulling();

	// Translate the sky dome to be centered around the camera position.
	worldMatrix = XMMatrixTranslation(cameraPosition.x, cameraPosition.y, cameraPosition.z);

	switch (m_cubemapsky)
	{
	case 0:
		Direct3D->TurnZBufferOff();
		// Render the sky dome using the sky dome shader.
		m_SkyDome->Render(Direct3D->GetDeviceContext());
		result = ShaderManager->RenderSkyDomeShader(Direct3D->GetDeviceContext(), m_SkyDome->GetIndexCount(), worldMatrix, viewMatrix,
			projectionMatrix, m_SkyDome->GetApexColor(), m_SkyDome->GetCenterColor());
		if (!result)
		{
			return false;
		}
		break;
	case 1:
		Direct3D->TurnDepthStencilStateLessEqual();
		// Render the sky cube using the sky cube shader.
		m_DayLightSkyCube->Render(Direct3D->GetDeviceContext());
		result = ShaderManager->RenderSkyCubeShader(Direct3D->GetDeviceContext(), m_DayLightSkyCube->GetIndexCount(), worldMatrix, viewMatrix,
			projectionMatrix, m_DayLightSkyCube->GetTextureCube());
		if (!result)
		{
			return false;
		}
		break;
	case 2:
		Direct3D->TurnDepthStencilStateLessEqual();
		// Render the sky cube using the sky cube shader.
		m_SunsetSkyCube->Render(Direct3D->GetDeviceContext());
		result = ShaderManager->RenderSkyCubeShader(Direct3D->GetDeviceContext(), m_SunsetSkyCube->GetIndexCount(), worldMatrix, viewMatrix,
			projectionMatrix, m_SunsetSkyCube->GetTextureCube());
		if (!result)
		{
			return false;
		}
		break;
	case 3:
		Direct3D->TurnDepthStencilStateLessEqual();
		// Render the sky cube using the sky cube shader.
		m_DesertSkyCube->Render(Direct3D->GetDeviceContext());
		result = ShaderManager->RenderSkyCubeShader(Direct3D->GetDeviceContext(), m_DesertSkyCube->GetIndexCount(), worldMatrix, viewMatrix,
			projectionMatrix, m_DesertSkyCube->GetTextureCube());
		if (!result)
		{
			return false;
		}
		break;
	case 4:
		Direct3D->TurnDepthStencilStateLessEqual();
		// Render the sky cube using the sky cube shader.
		m_NebulaSkyCube->Render(Direct3D->GetDeviceContext());
		result = ShaderManager->RenderSkyCubeShader(Direct3D->GetDeviceContext(), m_NebulaSkyCube->GetIndexCount(), worldMatrix, viewMatrix,
			projectionMatrix, m_NebulaSkyCube->GetTextureCube());
		if (!result)
		{
			return false;
		}
		break;
	case 5:
		Direct3D->TurnDepthStencilStateLessEqual();
		// Render the sky cube using the sky cube shader.
		m_PlanetSkyCube->Render(Direct3D->GetDeviceContext());
		result = ShaderManager->RenderSkyCubeShader(Direct3D->GetDeviceContext(), m_PlanetSkyCube->GetIndexCount(), worldMatrix, viewMatrix,
			projectionMatrix, m_PlanetSkyCube->GetTextureCube());
		if (!result)
		{
			return false;
		}
		break;
	default:
		break;
	}
	

	// Reset the world matrix.
	Direct3D->GetWorldMatrix(worldMatrix);

	// Turn the Z buffer back and back face culling on.
	Direct3D->TurnZBufferOn();
	Direct3D->TurnOnCulling();

	// Turn on wire frame rendering of the terrain if needed.
	if (m_wireFrame)
	{
		Direct3D->EnableWireframe();
	}


	Direct3D->GetWorldMatrix(worldMatrix);


	// Render the terrain cells (and cell lines if needed).
	for (i = 0; i < m_Terrain->GetCellCount(); i++)
	{
		// Render each terrain cell if it is visible only.
		result = m_Terrain->RenderCell(Direct3D->GetDeviceContext(), i, m_Frustum);
		if (result)
		{
			// Render the cell buffers using the terrain shader.
			result = ShaderManager->RenderTerrainShader(Direct3D->GetDeviceContext(), m_Terrain->GetCellIndexCount(i), worldMatrix, viewMatrix,
				projectionMatrix, TextureManager->GetTexture(0), TextureManager->GetTexture(1),
				m_Light->GetDirection(), m_Light->GetDiffuseColor());
			/*result = ShaderManager->RenderTerrainShader(Direct3D->GetDeviceContext(), m_Terrain->GetCellIndexCount(i), worldMatrix, viewMatrix,
				projectionMatrix, TextureManager->GetTexture(0), m_RenderTexture->GetShaderResourceView(),
				m_Light->GetDirection(), m_Light->GetDiffuseColor());*/
			/*result = ShaderManager->RenderShadowShader(Direct3D->GetDeviceContext(), m_Terrain->GetCellIndexCount(i), worldMatrix, viewMatrix, projectionMatrix, lightViewMatrix,
				lightOrthoMatrix, TextureManager->GetTexture(0), m_RenderTexture->GetShaderResourceView(), m_Light->GetDirection(),
				m_Light->GetAmbientColor(), m_Light->GetDiffuseColor());*/
			if (!result)
			{
				return false;
			}

			// If needed then render the bounding box around this terrain cell using the color shader. 
			if (m_cellLines)
			{
				m_Terrain->RenderCellLines(Direct3D->GetDeviceContext(), i);
				ShaderManager->RenderColorShader(Direct3D->GetDeviceContext(), m_Terrain->GetCellLinesIndexCount(i), worldMatrix,
					viewMatrix, projectionMatrix);
				if (!result)
				{
					return false;
				}
			}
		}
	}

	//// Render the terrain grid using the color shader.
	//m_Terrain->Render(Direct3D->GetDeviceContext());
	//result = result = ShaderManager->RenderTerrainShader(Direct3D->GetDeviceContext(), m_Terrain->GetIndexCount(), worldMatrix, viewMatrix,
	//	projectionMatrix, TextureManager->GetTexture(0), TextureManager->GetTexture(1),
	//	m_Light->GetDirection(), m_Light->GetDiffuseColor());
	//if (!result)
	//{
	//	return false;
	//}



	modelPosition = worldMatrix;
	modelPosition = XMMatrixTranslation(cubeTranslation[0], cubeTranslation[1], cubeTranslation[2]);

	m_Model->Render(Direct3D->GetDeviceContext());
	result = ShaderManager->RenderLightShader(Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), m_Model->GetTextureVector(), modelPosition, viewMatrix,
		projectionMatrix, m_Camera->GetPosition(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Light->GetDirection(), m_Light->GetSpecularPower(), m_Light->GetSpecularColor());

	/*result = ShaderManager->RenderShadowShader(Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), modelPosition, viewMatrix, projectionMatrix, lightViewMatrix,
		lightOrthoMatrix, m_Model->GetTextureVector()[0], m_RenderTexture->GetShaderResourceView(), m_Light->GetDirection(),
		m_Light->GetAmbientColor(), m_Light->GetDiffuseColor());*/


	modelPosition = worldMatrix;
	modelPosition = XMMatrixTranslation(10.0f, 0.0f, 30.0f);
	XMMATRIX meshModelScale = XMMatrixScaling(0.02f, 0.02f, 0.02f);
	modelPosition = meshModelScale * modelPosition;

	//m_MeshModel->Draw(ShaderManager, modelPosition, viewMatrix, projectionMatrix,m_Camera->GetPosition(),m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Light->GetDirection(), m_Light->GetSpecularPower(), m_Light->GetSpecularColor(), 0.2f, mCharacterInstance1.Model->DiffuseMapSRV[0], mCharacterInstance1.Model->NormalMapSRV[0]);
	//
	deviceContext->PSSetShader(pixelshader.GetShader(), NULL, 0);
	deviceContext->VSSetShader(d3dvertexshader_animation.get()->GetShader(device.Get()), NULL, 0);
	deviceContext->IASetInputLayout(d3dvertexshader_animation.get()->GetLayout());

	cb_ps_light.data.ambientLightColor = XMFLOAT3(m_Light->GetAmbientColor().x, m_Light->GetAmbientColor().y, m_Light->GetAmbientColor().z);
	cb_ps_light.data.ambientLightStrength = 1.0f;
	cb_ps_light.data.dynamicLightColor = XMFLOAT3(m_Light->GetDiffuseColor().x, m_Light->GetDiffuseColor().y, m_Light->GetDiffuseColor().z);
	cb_ps_light.data.dynamicLightStrength = 1.0f;
	cb_ps_light.data.lightPosition = m_Light->GetPosition();
	cb_ps_light.data.dynamicLightAttenuation_a = 1.0f;
	cb_ps_light.data.dynamicLightAttenuation_b = 1.0f;
	cb_ps_light.data.dynamicLightAttenuation_c = 1.0f;
	cb_ps_light.ApplyChanges();

	deviceContext->PSSetShaderResources(4, 1, m_RenderTexture->GetShaderResourceViewAddress());

	m_MeshModel->SwitchAnim(0);
	m_MeshModel->Draw(modelPosition, viewMatrix, projectionMatrix);


	modelPosition = worldMatrix;
	modelPosition = XMMatrixTranslation(15.0f, 0.0f, 30.0f);
	meshModelScale = XMMatrixScaling(0.02f, 0.02f, 0.02f);
	modelPosition = meshModelScale * modelPosition;
	
	m_MeshModel->SwitchAnim(1);
	m_MeshModel->Draw(modelPosition, viewMatrix, projectionMatrix);

	modelPosition = worldMatrix;
	modelPosition = XMMatrixTranslation(20.0f, 0.0f, 30.0f);
	meshModelScale = XMMatrixScaling(0.02f, 0.02f, 0.02f);
	modelPosition = meshModelScale * modelPosition;

	m_MeshModel->SwitchAnim(2);
	m_MeshModel->Draw(modelPosition, viewMatrix, projectionMatrix);
	
	
	modelPosition = worldMatrix;
	modelPosition = XMMatrixTranslation(25.0f, 0.0f, 30.0f);
	meshModelScale = XMMatrixScaling(0.02f, 0.02f, 0.02f);
	modelPosition = meshModelScale * modelPosition;

	m_MeshModel->SwitchAnim(3);
	m_MeshModel->Draw(modelPosition, viewMatrix, projectionMatrix);
	//float Deltatime = 0.01f;
	
	XMMATRIX modelScale = XMMatrixScaling(0.05f, 0.05f, -0.05f);
	XMMATRIX modelRot = XMMatrixRotationY(mCharacterInstance1.position->GetRotationY() * 0.0174532925f);
	XMFLOAT3 position = mCharacterInstance1.position->GetPosition();
	XMMATRIX modelOffset = XMMatrixTranslation(position.x, position.y, position.z);
	//get model world matrix
	//modelPosition = XMLoadFloat4x4(&mCharacterInstance1.World);
	modelPosition = modelScale * modelRot * modelOffset;
	


	//render model for each subset
	for (UINT subset = 0; subset < mCharacterInstance1.Model->SubsetCount; ++subset)
	{
		
		ShaderManager->RenderSkeletalCharacterShader(Direct3D->GetDeviceContext(), mCharacterInstance1.FinalTransforms.size(), modelPosition, viewMatrix, projectionMatrix,
			mCharacterInstance1.Model->DiffuseMapSRV[subset], mCharacterInstance1.Model->NormalMapSRV[subset], m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Light->GetDirection(), m_Camera->GetPosition(), &mCharacterInstance1.FinalTransforms[0], mCharacterInstance1.Model->Mat[subset]);

		//DrawCall
		mCharacterInstance1.Model->ModelMesh.Draw(Direct3D->GetDeviceContext(), subset);
	}



	position = mCharacterInstance2.position->GetPosition();
	modelOffset = XMMatrixTranslation(position.x, position.y, position.z);
	modelPosition = modelScale * modelOffset;

	//render model for each subset
	for (UINT subset = 0; subset < mCharacterInstance2.Model->SubsetCount; ++subset)
	{

		ShaderManager->RenderSkeletalCharacterShader(Direct3D->GetDeviceContext(), mCharacterInstance2.FinalTransforms.size(), modelPosition, viewMatrix, projectionMatrix,
			mCharacterInstance2.Model->DiffuseMapSRV[subset], mCharacterInstance2.Model->NormalMapSRV[subset], m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Light->GetDirection(), m_Camera->GetPosition(), &mCharacterInstance2.FinalTransforms[0], mCharacterInstance2.Model->Mat[subset]);

		//DrawCall
		mCharacterInstance2.Model->ModelMesh.Draw(Direct3D->GetDeviceContext(), subset);
	}



	
	Direct3D->TurnOnParticleBlending();
	// Put the particle system vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_ParticleSystem->Render(Direct3D->GetDeviceContext());
	
	Direct3D->GetWorldMatrix(worldMatrix);
	XMMATRIX particlePosition;
	position = mCharacterInstance1.position->GetPosition();

	double angle = atan2(position.x - cameraPosition.x, position.z - cameraPosition.z) * (180.0 / XM_PI);
	float rotation = (float)angle * 0.0174532925f;

	particlePosition = worldMatrix;
	//particlePosition *= XMMatrixRotationY(rotation);
	particlePosition = XMMatrixTranslation(255.0f, 3.0f, 255.0f);
	//particlePosition *= XMMatrixTranslation(position.x, position.y + 2.0f, position.z);

	

	// Render the particle using the texture shader.
	result = ShaderManager->RenderParticleShader(Direct3D->GetDeviceContext(), m_ParticleSystem->GetIndexCount(), particlePosition, viewMatrix, projectionMatrix,
		m_ParticleSystem->GetTexture());
	if (!result)
	{
		return false;
	}
	
	// Turn off alpha blending.
	Direct3D->TurnOffParticleBlending();


	// Turn off the wire frame rendering once the terrain rendering is complete so we don't render anything else such as the UI in wire frame.
	// Turn off wire frame rendering of the terrain if it was on.
	if (m_wireFrame)
	{
		Direct3D->DisableWireframe();
	}
	Direct3D->GetWorldMatrix(worldMatrix);
	// Update the render counts in the UI.
	result = m_UserInterface->UpdateRenderCounts(Direct3D->GetDeviceContext(), m_Terrain->GetRenderCount(), m_Terrain->GetCellsDrawn(),
		m_Terrain->GetCellsCulled());
	if (!result)
	{
		return false;
	}

	// Render the user interface.
	if (m_displayUI)
	{
		result = m_UserInterface->Render(Direct3D, ShaderManager, worldMatrix, baseViewMatrix, orthoMatrix);
		if (!result)
		{
			return false;
		}
	}

	

	return true;
}

bool ZoneClass::InitializeShaders()
{
	std::wstring shaderfolder = L"";
#pragma region DetermineShaderPath
	if (IsDebuggerPresent())
	{
#ifdef _DEBUG // Debug Mode
#ifdef _WIN64 // x64
		shaderfolder = L"..\\x64\\Debug\\";
#else	// x86
		shaderfolder = L"..\\Debug\\";
#endif
#else	// Release Mode
#ifdef _WIN64
		shaderfolder = L"..\\x64\\Release\\";
#else	// x86
		shaderfolder = L"..\\Release\\";
#endif
#endif
	}



	d3dvertexshader = std::make_unique<D3DVertexShader>(device.Get(), StringHelper::WideToString(shaderfolder) + "vertexShader.cso");
	d3dvertexshader_animation = std::make_unique<D3DVertexShader>(device.Get(), StringHelper::WideToString(shaderfolder) + "VertexShaderAnim.cso");
	d3dvertexshader_nolight = std::make_unique<D3DVertexShader>(device.Get(), StringHelper::WideToString(shaderfolder) + "VS_nolight.cso");
	d3dvertexshader_shadowmap = std::make_unique<D3DVertexShader>(device.Get(), StringHelper::WideToString(shaderfolder) + "VS_shadowmap.cso");
	d3dvertexshader_shadowmap_anim = std::make_unique<D3DVertexShader>(device.Get(), StringHelper::WideToString(shaderfolder) + "VS_shadowmap_anim.cso");
	if (!pixelshader.Initialize(this->device, shaderfolder + L"pixelshader.cso"))
	{
		return false;
	}
	if (!pixelshader_nolight.Initialize(this->device, shaderfolder + L"pixelshader_nolight.cso"))
	{
		return false;
	}
	if (!pixelshader_tonemapping.Initialize(this->device, shaderfolder + L"pixelshader_tonemapping.cso"))
	{
		return false;
	}
	if (!pixelshader_heightmapping.Initialize(this->device, shaderfolder + L"PixelShader_HeightMapping.cso"))
	{
		return false;
	}
}

IVertexShader* ZoneClass::CreateVertexShader(const std::string& filename)
{
	return new D3DVertexShader(device.Get(), filename);
}

void ZoneClass::SetLight()
{
	cb_ps_camera.data.cameraPosition = m_Camera->GetPosition();
	cb_ps_camera.ApplyChanges();
	deviceContext->PSSetConstantBuffers(1, 1, cb_ps_camera.GetAddress());

	//cb_ps_light.data.dynamicLightColor = m_Light->GetDiffuseColor();
	//cb_ps_light.data.dynamicLightStrength = light.lightStrenght;
	//cb_ps_light.data.dynamicPosition = light.GetPositionFloat3();
	//cb_ps_light.data.dynamicLightAttenuation_a = light.attenuation_a;
	//cb_ps_light.data.dynamicLightAttenuation_b = light.attenuation_b;
	//cb_ps_light.data.dynamicLightAttenuation_c = light.attenuation_c;

	//cb_ps_light.ApplyChanges();
	//deviceContext->PSSetConstantBuffers(0, 1, cb_ps_light.GetAddress());
}
