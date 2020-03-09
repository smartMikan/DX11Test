#pragma once

const int SHADOWMAP_WIDTH = 1024;
const int SHADOWMAP_HEIGHT = 1024;
const float SHADOWMAP_DEPTH = 50.0f;
const float SHADOWMAP_NEAR = 1.0f;

#include<memory>

#include "Engine/d3dclass.h"
#include "Engine/inputclass.h"
#include "Graphic/RasterTekObjects/shadermanagerclass.h"
#include "texturemanagerclass.h"
#include "Engine/timerclass.h"
#include "Graphic/RasterTekObjects/userinterfaceclass.h"
#include "Engine/cameraclass.h"
#include "Engine/positionclass.h"
#include "Engine/GameObjectClass.h"
#include "Engine/TerrainClass.h"
#include "Engine/frustumclass.h"
#include "Graphic/RasterTekObjects/skydomeclass.h"
#include "Graphic/RasterTekObjects/skycubeclass.h"
#include "Engine/lightclass.h"
#include "Graphic/RasterTekObjects/particlesystemclass.h"


#include "Graphic/RasterTekObjects/rendertextureclass.h"
#include "Graphic/Shaders.h"

class ZoneClass
{
public:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;


	ZoneClass();
	ZoneClass(const ZoneClass&);
	~ZoneClass();

	bool Initialize(D3DClass*, HWND, int, int, float);
	void Shutdown();
	bool Frame(D3DClass*, InputClass*, ShaderManagerClass*, TextureManagerClass*, float frametime, int fps, int cpu);



public:
	float wallTranslation[3] = { 12.0f,0.1f,33.0f };
	float wallRotation[3] = { 0.0f, 0.0f, 0.0f };
	float wallScaling[3] = { 20.0f, 20.0f, 1.0f };
	
	int m_lightType = 0;
	bool toonShading = true;
	bool m_displayUI = false;

	IVertexShader* CreateVertexShader(const std::string& filename);

	LightClass* m_Light;
	//SkinnedModelInstance mCharacterInstance2;
	GameObjectClass* m_AnimModel;
	GameObjectClass* m_UnMoveModel;
	GameObjectClass* m_Player;
	GameObjectClass* m_tianyi;
	GameObjectClass* m_sword;
	
private:
	void HandleMovementInput(InputClass*, float frameTime,float fps);
	
	bool RenderShadowMap(D3DClass* Direct3D, const XMMATRIX&, const XMMATRIX&);
	
	bool Render(D3DClass*, ShaderManagerClass*, TextureManagerClass*);
	
	bool RenderSky(D3DClass*, ShaderManagerClass*, const XMMATRIX&, const XMMATRIX&, const XMMATRIX&);
	
	bool RenderParticles(D3DClass*, ShaderManagerClass*, const XMMATRIX&, const XMMATRIX&, const XMMATRIX&);

	bool RenderTerrain(D3DClass*, ShaderManagerClass*, TextureManagerClass* , const XMMATRIX&, const XMMATRIX&, const XMMATRIX&);

	bool RenderAnimationGameObjects(D3DVertexShader*, const XMMATRIX& ,const XMMATRIX&);
	bool RenderNonAnimationGameObjects(D3DVertexShader*, const XMMATRIX& ,const XMMATRIX&);

	bool InitializeShaders();
	void SetLight(int lightType);
private:
	ComPtr<ID3D11Device> device;
	ComPtr <ID3D11DeviceContext> deviceContext;

	UserInterfaceClass * m_UserInterface;
	CameraClass* m_Camera;
	PositionClass* m_Position;
	TerrainClass* m_Terrain;
	FrustumClass* m_Frustum;
	SkyDomeClass* m_SkyDome;
	SkyCubeClass* m_DayLightSkyCube;
	SkyCubeClass* m_SunsetSkyCube;
	SkyCubeClass* m_DesertSkyCube;
	SkyCubeClass* m_NebulaSkyCube;
	SkyCubeClass* m_PlanetSkyCube;
	//ModelClass* m_Model;
	


	//M3d Character Model Data
	//SkinnedModelClass* mCharacterModel;

	//Character Instance
	//SkinnedModelInstance mCharacterInstance1;
	
	ParticleSystemClass* m_ParticleSystem;

	RenderTextureClass* m_RenderTexture;


	//bool m_particleFollow = true;

	bool m_wireFrame = false, m_cellLines = false, m_heightLocked = false;
	int m_cubemapsky = 0;

	XMMATRIX modelPosition;

	// shaders
	std::unique_ptr<D3DVertexShader> d3dvertexshader;
	std::unique_ptr<D3DVertexShader> d3dvertexshader_animation;
	std::unique_ptr<D3DVertexShader> d3dvertexshader_nolight;
	std::unique_ptr<D3DVertexShader> d3dvertexshader_shadowmap;
	std::unique_ptr<D3DVertexShader> d3dvertexshader_shadowmap_anim;
	PixelShader pixelshader;
	PixelShader pixelshader_nolight;
	PixelShader pixelshader_toonmapping;
	PixelShader pixelshader_heightmapping;
	PixelShader pixelshader_depthColor;

	
	// c_buffers
	ConstantBuffer<CB_VS_MatrixBuffer> cb_vs_wvpBuffer;
	ConstantBuffer<CB_PS_LightBuffer> cb_ps_light;
	ConstantBuffer<CB_PS_CameraBuffer> cb_ps_camera;
	ConstantBuffer<CB_PS_Material> cb_ps_material;
	ConstantBuffer<CB_PS_ShadowMatrix> cb_ps_shadowMatrix;
	ConstantBuffer<ConstantBuffer_Bones> cb_bones;

};
