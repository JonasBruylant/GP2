#pragma once
#include "GameScene.h"


class W2_AssignmentScene final : public GameScene
{
public:
	W2_AssignmentScene() :GameScene(L"W1_AssignmentScene") {}
	~W2_AssignmentScene() override = default;
	W2_AssignmentScene(const W2_AssignmentScene& other) = delete;
	W2_AssignmentScene(W2_AssignmentScene&& other) noexcept = delete;
	W2_AssignmentScene& operator=(const W2_AssignmentScene& other) = delete;
	W2_AssignmentScene& operator=(W2_AssignmentScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;

	void onTrigger(PxTriggerPair* pairs, PxU32 count);

private:

//Game objects
	GameObject* m_pBall = nullptr;
	GameObject* m_pCube1 = nullptr;
	GameObject* m_pCube2 = nullptr;
	PxRigidDynamic* m_pBallRigid = nullptr;
	GameObject* m_pLevelTriangle = nullptr;

	GameObject* m_pHingeCube1 = nullptr;
	GameObject* m_pHingeCube2 = nullptr;
	GameObject* m_pBall2 = nullptr;
	GameObject* m_pBall3 = nullptr;
	

//Trigger
	PxRigidStatic* m_pTriggerLeft = nullptr;
	PxRigidStatic* m_pTriggerRight = nullptr;

//Sounds
	FMOD::Channel* m_pChannel2D = nullptr;
};
