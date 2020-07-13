#include "GameObjectClass.h"


GameObjectClass::GameObjectClass()
{
	m_Model = 0;
}

GameObjectClass::GameObjectClass(const GameObjectClass& other)
{

}

GameObjectClass::~GameObjectClass()
{
	Shutdown();
}

bool GameObjectClass::Initialize(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext,
	ConstantBuffer<CB_VS_MatrixBuffer>& wvpMatrix, ConstantBuffer<CB_PS_Material>& cb_ps_material, IVertexShader* pVertexShader)
{
	bool result;
	this->deviceContext = deviceContext;
	m_Model = new Model;
	if (!m_Model) {
		return false;
	}

	result = m_Model->Initialize(filePath, device, deviceContext, wvpMatrix, cb_ps_material, pVertexShader);
	if (result == false) {
		return false;
	}


	// Set the initial position and rotation.
	m_Transform.SetPosition(10.0f, 0.0f, 10.0f);
	m_Transform.SetRotation(0.0f, 0.0f, 0.0f);
	return true;
}

bool GameObjectClass::InitAnimation(ConstantBuffer<ConstantBuffer_Bones>& cbufBones)
{
	mAnimComp = std::make_unique<AnimationComponent>(&m_Animator);
	
	mPlayAnimtion = true;
	mAnimTimers.emplace_back(new Timer);
	mAnimTimers[0]->Start();
	//mAnimTimer.Start();

	return m_Model->InitAnimation(&cbufBones, &m_Animator, mAnimComp.get());
}

bool GameObjectClass::AddAnimation(const std::string & filePath, bool disablerootTrans, bool disablerootRot, bool disablerootScale)
{
	assert(mAnimComp.get() != nullptr);

	mAnimTimers.emplace_back(new Timer);
	
	mAnimTimers.back()->Start();

	return m_Model->AddAnimation(filePath, &m_Animator, mAnimComp.get(),disablerootTrans, disablerootRot, disablerootScale);
}

int GameObjectClass::GetAnimCount()
{
	return m_Animator.GetNumAnimations();
}

bool GameObjectClass::InitBakedAnim(ConstantBuffer<ConstantBuffer_BakedBones>& cbufBones)
{
	m_baked_Animator = BakedAnimator(&cbufBones);

	return true;
}

bool GameObjectClass::LoadBakedAnim(const std::string& filename)
{

	return m_baked_Animator.LoadAnimFromFile(filename);
}

int GameObjectClass::GetBakedAnimCount()
{
	return m_baked_Animator.bakedclips.size();
}



void GameObjectClass::Shutdown()
{
	// Release the position object.
	if (m_Model)
	{
		delete m_Model;
		m_Model = 0;
	}

	if (mAnimComp) {
		mAnimComp.release();
		
	}

}

void GameObjectClass::Frame(float frametime)
{
	m_Transform.SetFrameTime(frametime);
}

double GameObjectClass::Render(const XMMATRIX & viewMatrix, const XMMATRIX & projectionMatrix,bool sameanim)
{	
	return Draw(m_Transform.GetWorldMatrix(), viewMatrix, projectionMatrix,sameanim);
}


double GameObjectClass::RenderWithBakedAnim(const XMMATRIX& viewMatrix, const XMMATRIX& projectionMatrix, int animnum, float& timepos)
{
	double drawTime, calanimtime;
	LARGE_INTEGER t1, t2, tc;
	QueryPerformanceFrequency(&tc);
	QueryPerformanceCounter(&t1);

	if (mPlayAnimtion)
	{
		if (timepos >= m_baked_Animator.bakedclips[animnum].animduration)
		{
			timepos = 0;
		}
	}
	

	m_baked_Animator.Bind(deviceContext, animnum, timepos);



	QueryPerformanceCounter(&t2);
	calanimtime = (double)(t2.QuadPart - t1.QuadPart) / (double)tc.QuadPart;



	drawTime = m_Model->Draw(m_Transform.GetWorldMatrix(), viewMatrix, projectionMatrix);

	return calanimtime + drawTime;
}

double GameObjectClass::Draw(const XMMATRIX & worldMatrix, const XMMATRIX & viewMatrix, const XMMATRIX & projectionMatrix,bool sameanim)
{
	double drawTime, calanimtime;

	LARGE_INTEGER t1,t2,tc;
    QueryPerformanceFrequency(&tc);
    QueryPerformanceCounter(&t1);

	int index = m_Animator.GetCurrentAnimationIndex();
	if (!sameanim) {
		if (mPlayAnimtion)
		{
			if ((float)mAnimTimers[index]->GetMiliseceondsElapsed() / (1000.0f / mAnimTimeScale) >= m_Animator.GetCurrentAnimation().duration)
				mAnimTimers[index]->Restart();
			m_Animator.SetTimpPos((float)mAnimTimers[index]->GetMiliseceondsElapsed() / (1000.0f / mAnimTimeScale));

			const BoneChannel* channel = mAnimComp->GetCurrentBoneChannel();
			if (channel)
			{
				m_Animator.Bind(deviceContext);
			}
		}
	}

    QueryPerformanceCounter(&t2);
	calanimtime =(double)(t2.QuadPart-t1.QuadPart)/(double)tc.QuadPart;
    
	drawTime = m_Model->Draw(worldMatrix, viewMatrix, projectionMatrix);

	return calanimtime > drawTime ? calanimtime: drawTime;
}

void GameObjectClass::SwitchAnim(int index)
{
	assert(index < m_Animator.GetNumAnimations(), "AnimationOverHeads!");
	if (m_Animator.GetCurrentAnimationIndex() == index) {
		return;
	}
	else
	{
		m_Animator.SetCurrentAnimationIndex(index);
	}
}

void GameObjectClass::StartAnim(int index)
{
	SwitchAnim(index);
	mAnimTimers[index]->Restart();
}


XMMATRIX GameObjectClass::GetWorldMatrix()
{
	return XMMatrixTranslation(m_Transform.GetPosition().x, m_Transform.GetPosition().y, m_Transform.GetPosition().z);
}

bool GameObjectClass::SetWorldMatrix(XMMATRIX world)
{
	XMFLOAT4X4 pos;
	XMStoreFloat4x4(&pos, world);
	m_Transform.SetPosition(pos._14,pos._24,pos._34);
	return true;
}





