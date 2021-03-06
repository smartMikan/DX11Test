#include "Player.h"
#include "Engine/inputclass.h"


Player::Player(MainGame* game):GameObject(game)
{
}

bool Player::InitialAnimations(ConstantBuffer<ConstantBuffer_Bones>& cb_Bones)
{
	bool result;

	result = InitAnimation(cb_Bones);
	if (!result)
	{
		ErrorLoger::Log("Could not initialize the player baseAnimation.");
		return false;
	}
	result = AddAnimation("./3DModel/SwordPack/swordandshieldidle.fbx");
	if (!result)
	{
		ErrorLoger::Log("Could not Add swordandshieldidle.fbx Animation.");
		return false;
	}
	result = AddAnimation("./3DModel/SwordPack/swordandshieldrun.fbx", true);
	if (!result)
	{
		ErrorLoger::Log("Could not Add swordandshieldrun.fbx Animation.");
		return false;
	}
	result = AddAnimation("./3DModel/SwordPack/swordandshieldslash.fbx", true);
	if (!result)
	{
		ErrorLoger::Log("Could not Add swordandshieldslash.fbx Animation.");
		return false;
	}
	result = AddAnimation("./3DModel/SwordPack/swordandshieldjump.fbx", true);
	if (!result)
	{
		ErrorLoger::Log("Could not Add swordandshieldjump.fbx Animation.");
		return false;
	}
	result = AddAnimation("./3DModel/SwordPack/sword and shield run (2).fbx", true);
	if (!result)
	{
		ErrorLoger::Log("Could not Add sword and shield run (2).fbx Animation.");
		return false;
	}
	result = AddAnimation("./3DModel/SwordPack/sword and shield 180 turn.fbx", false, true);
	if (!result)
	{
		ErrorLoger::Log("Could not Add sword and shield 180 turn.fbx Animation.");
		return false;
	}
	result = AddAnimation("./3DModel/SwordPack/sword and shield 180 turn (2).fbx", true, true);
	if (!result)
	{
		ErrorLoger::Log("Could not /sword and shield 180 turn (2).fbx Animation.");
		return false;
	}

	return result;
}


void Player::Frame(float frametime)
{
	GameObject::Frame(frametime);
	if (isCyclone) {
		Round(30);
	}
}

bool Player::HandleInput(InputClass* Input, Transform * orbitTrans)
{
	bool keyDown = false;
	//reset player animation to idle
	if (!isAttack && !isJump) {
		SwitchAnim(1);
	}

	//attack
	keyDown = Input->GetMouseButtonDown(0) || Input->IsKeyPressed(DIK_LCONTROL);
	if (keyDown)
	{
		//switch the model animation to attack
		StartAnim(1);

		//if is attacking,continue the animation
		if (isAttack) {
			SwitchAnim(3);
		}
		else
		{
			//reset attack animation
			StartAnim(3);
			isAttack = true;
		}
	}
	else
	{
		isAttack = false;
	}


	keyDown = Input->GetMouseButton(1);
	if (keyDown)
	{

		//if is attacking,continue the animation
		if (isCyclone)
		{
			SwitchAnim(7);
		}
		else
		{
			//start cyclone animation
			StartAnim(7);
			SwitchAnim(7);
			isCyclone = true;
		}
	}
	else
	{
		isCyclone = false;
	}





	//left
	keyDown = Input->IsKeyPressed(DIK_LEFT) || Input->IsKeyPressed(DIK_A);
	m_Transform.TurnLeft(keyDown);
	orbitTrans->Orbit(keyDown, true, m_Transform.GetPosition());

	if (keyDown && !isJump) {
		//switch the model animation to turnleft & reset idle animation
		StartAnim(1);
		SwitchAnim(6);
		isAttack = false;
	}

	//right
	keyDown = Input->IsKeyPressed(DIK_RIGHT) || Input->IsKeyPressed(DIK_D);
	m_Transform.TurnRight(keyDown);
	orbitTrans->Orbit(keyDown, false, m_Transform.GetPosition());

	if (keyDown && !isJump) {
		//switch the model animation to turnright & reset idle animation
		StartAnim(1);
		SwitchAnim(6);
		isAttack = false;
	}

	//forward
	keyDown = Input->IsKeyPressed(DIK_UP) || Input->IsKeyPressed(DIK_W);
	m_Transform.MoveForward(keyDown);

	if (keyDown && !isJump) {
		//switch the model animation to moveforward & reset idle animation
		StartAnim(1);
		SwitchAnim(2);
		isAttack = false;
	}

	//back
	keyDown = Input->IsKeyPressed(DIK_DOWN) || Input->IsKeyPressed(DIK_S);
	m_Transform.MoveBackward(keyDown);

	if (keyDown && !isJump) {

		//switch the model animation to movebackward & reset idle animation
		StartAnim(1);
		SwitchAnim(5);
		isAttack = false;
	}

	//jump
	if (Input->IsKeyToggled(DIK_SPACE) && !isJump)
	{
		//switch the model animation to jump & reset idle animation
		StartAnim(1);
		SwitchAnim(4);
		isAttack = false;
		Jump(10, 25);
	}


	return true;
}

void Player::Round(float speed)
{
	m_Transform.TurnRight(speed);

}


float Player::GetJumpHeight()
{
	float t = (float)m_jumpTimer.GetMiliseceondsElapsed() / (1000.0f);
	float s = m_jumpSpeed * t - 0.5f * m_jumpAcce * t * t;
	if (s < 0) {
		s = 0;
		isJump = false;
		m_jumpAcce = 0;
		m_jumpSpeed = 0;
		m_jumpTimer.Stop();
	}
	return  s;
}

float Player::Jump(float startSpeed, float acce)
{
	m_jumpAcce = acce;
	m_jumpSpeed = startSpeed;
	m_jumpTimer.Restart();
	isJump = true;
	return 0.0f;
}

