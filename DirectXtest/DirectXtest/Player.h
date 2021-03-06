#pragma once
#include "Engine/GameObjectClass.h"
class Player : public GameObject
{
public:
    Player(class MainGame* game);


    bool InitialAnimations(ConstantBuffer<ConstantBuffer_Bones>& cb_Bones);

    void Frame(float frametime);

    bool HandleInput(class InputClass* Input, Transform* orbitTrans);

    void Round(float speed);

    float GetJumpHeight();
    float Jump(float startSpeed, float acce);

    Timer m_jumpTimer;
    bool isCyclone = false;
    bool isJump;
    bool isGround;
    bool isAttack;
    float attackDuration;
    float m_jumpAcce = 0;
    float m_jumpSpeed = 0;
};

