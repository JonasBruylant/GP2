#include "stdafx.h"
#include "W1_AssignmentScene.h"
#include "CubePosColorNorm.h"
#include "SpherePosColorNorm.h"

enum class InputIds : int
{
	Forward,
	Backward,
	Left,
	Right
};


void W1_AssignmentScene::Initialize()
{
	EnablePhysxDebugRendering(true);


	const auto pPhysX = PhysxManager::GetInstance()->GetPhysics();
	const PxMaterial* pCubeMaterial = pPhysX->createMaterial(.5f, .7f, .8f);
	const PxMaterial* pSphereMaterial = pPhysX->createMaterial(.5f, .95f, .8f);
	const PxMaterial* pPlaneMaterial = pPhysX->createMaterial(.0f, .0f, .0f);

    const float cubeSize{ 2.5f };
    const XMFLOAT3 actorDimension{ cubeSize, cubeSize, cubeSize };
    
	const int rows = 5;
	const int cols = 5;
	const float offset = 1;
	for (size_t i = 0; i < rows * cols; i++)
	{

		int randomDegree = (rand() % 31) - 15; //-15 -> 15
		int row = i % cols;
		int col = i / cols;
		GameObject* pCube = new CubePosColorNorm(actorDimension.x, actorDimension.y, actorDimension.z);
		AddGameObject(pCube);

		pCube->Translate(row * (actorDimension.x + offset), col * (actorDimension.y + offset / 8) +1, 0);
		pCube->Rotate(0, randomDegree, 0);
		
		
		//Cube actor (Makes it so the cube has collision)
		PxRigidDynamic* pCubeActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
		PxBoxGeometry boxGeometry = PxBoxGeometry{ actorDimension.x / 2.f, actorDimension.y / 2.f, actorDimension.z / 2.f };
		
		//PxShape* pCubeShape = pPhysX->createShape(boxGeometry, *pDefaultMaterial, true);
		//pCubeActor->attachShape(*pCubeShape);
		//
		//m_pPhysxScene->addActor(*pCubeActor);
		
		PxRigidActorExt::createExclusiveShape(*pCubeActor, boxGeometry, *pCubeMaterial);
		
		//Link CUBE Rigid with CUBE_ACTOR
		pCube->AttachRigidActor(pCubeActor);
		pCubeActor->setMass(.5f);

		m_pCubes.push_back(pCube);

	}


	//SPHERE OBJECT
	const float sphereRadius{ 2.f };
	const int sliceAndStacks{ 16 };
	m_pBall = new SpherePosColorNorm(sphereRadius, sliceAndStacks, sliceAndStacks, XMFLOAT4{ Colors::Azure });
	AddGameObject(m_pBall);

	m_pBall->Translate(0, 0, -10);
		//Sphere Actor
	
	PxRigidDynamic* pSphereActor = pPhysX->createRigidDynamic(PxTransform{PxIdentity});
	m_pBallRigid = pSphereActor;
	pSphereActor->setMass(10.f);

	PxSphereGeometry sphereGeometry = PxSphereGeometry{ sphereRadius };


	//PxShape* pSphereShape = pPhysX->createShape(sphereGeometry, *pDefaultMaterial, true);
	//pSphereActor->attachShape(*pSphereShape);
	//
	//m_pPhysxScene->addActor(*pSphereActor);

	//Does the same as the 3 lines above 
	PxRigidActorExt::createExclusiveShape(*pSphereActor, sphereGeometry, *pSphereMaterial);


	m_pBall->AttachRigidActor(pSphereActor);



	//GROUND PLATE
	PxRigidStatic* pPlaneActor = pPhysX->createRigidStatic(PxTransform{ PxQuat{PxPiDivTwo, PxVec3{0,0,1}	} });
	PxRigidActorExt::createExclusiveShape(*pPlaneActor, PxPlaneGeometry{}, *pPlaneMaterial);

	m_pPhysxScene->addActor(*pPlaneActor);



	//INPUT
	m_SceneContext.GetInput()->AddInputAction(InputAction{
	static_cast<int>(InputIds::Backward), InputTriggerState::down, VK_DOWN, -1, XINPUT_GAMEPAD_DPAD_DOWN });

	m_SceneContext.GetInput()->AddInputAction(InputAction{
	static_cast<int>(InputIds::Forward), InputTriggerState::down, VK_UP, -1, XINPUT_GAMEPAD_DPAD_DOWN });

	m_SceneContext.GetInput()->AddInputAction(InputAction{
	static_cast<int>(InputIds::Right), InputTriggerState::down, VK_RIGHT, -1, XINPUT_GAMEPAD_DPAD_DOWN });

	m_SceneContext.GetInput()->AddInputAction(InputAction{
	static_cast<int>(InputIds::Left), InputTriggerState::down, VK_LEFT, -1, XINPUT_GAMEPAD_DPAD_DOWN });
}

void W1_AssignmentScene::Update()
{
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, 'R'))
		m_IsInitialized = false;

	PxVec3 torqueForce{ 0,0,0 };
	float torquePowerMultiplier{ 5'000.f };
	
	
	//std::cout << m_SceneContext.GetCamera()->GetForward().x << " " << m_SceneContext.GetCamera()->GetForward().y << " " <<  m_SceneContext.GetCamera()->GetForward().z << '\n';

	if (m_SceneContext.GetInput()->IsActionTriggered(static_cast<int>(InputIds::Left)))
	{
		torqueForce.x = m_SceneContext.GetCamera()->GetForward().x * torquePowerMultiplier;
		torqueForce.y = m_SceneContext.GetCamera()->GetForward().y * torquePowerMultiplier;
		torqueForce.z = m_SceneContext.GetCamera()->GetForward().z * torquePowerMultiplier;
		
		m_pBallRigid->addTorque(torqueForce);
	}

	if (m_SceneContext.GetInput()->IsActionTriggered(static_cast<int>(InputIds::Right)))
	{
		torqueForce.x = -m_SceneContext.GetCamera()->GetForward().x * torquePowerMultiplier;
		torqueForce.y = -m_SceneContext.GetCamera()->GetForward().y * torquePowerMultiplier;
		torqueForce.z = -m_SceneContext.GetCamera()->GetForward().z * torquePowerMultiplier;
		
		m_pBallRigid->addTorque(torqueForce);
	}

	if (m_SceneContext.GetInput()->IsActionTriggered(static_cast<int>(InputIds::Forward)))
	{
		torqueForce.x = m_SceneContext.GetCamera()->GetRight().x * torquePowerMultiplier;
		torqueForce.y = m_SceneContext.GetCamera()->GetRight().y * torquePowerMultiplier;
		torqueForce.z = m_SceneContext.GetCamera()->GetRight().z * torquePowerMultiplier;

		m_pBallRigid->addTorque(torqueForce);
	}

	if (m_SceneContext.GetInput()->IsActionTriggered(static_cast<int>(InputIds::Backward)))
	{
		torqueForce.x = -m_SceneContext.GetCamera()->GetRight().x * torquePowerMultiplier;
		torqueForce.y = -m_SceneContext.GetCamera()->GetRight().y * torquePowerMultiplier;
		torqueForce.z = -m_SceneContext.GetCamera()->GetRight().z * torquePowerMultiplier;

		m_pBallRigid->addTorque(torqueForce);
	}

	//std::cout << torqueForce.x << " " << torqueForce.y << " " <<  torqueForce.z << '\n';	


	if (!m_IsInitialized)
	{
		const float cubeSize{ 2.5f };
		const int rows = 5;
		const int cols = 5;
		const float offset = 1;
		const XMFLOAT3 actorDimension{ cubeSize, cubeSize, cubeSize };

		for (size_t i = 0; i < rows * cols; i++)
		{

			int randomDegree = (rand() % 31) - 15; //-15 -> 15
			int row = i % cols;
			int col = i / cols;

			//m_pCubes[i]->Translate(row * (actorDimension.x + offset), col * (actorDimension.y + offset), 0);
			m_pCubes[i]->Translate(row * (actorDimension.x + offset), col * (actorDimension.y + offset / 8) + 1, 0);
			m_pCubes[i]->Rotate(0, randomDegree, 0);
		}

		m_pBall->Translate(0, 0, -10.f);
		m_IsInitialized = true;
	}
}

void W1_AssignmentScene::Draw() const
{
}

void W1_AssignmentScene::OnSceneActivated()
{
}

void W1_AssignmentScene::OnSceneDeactivated()
{
}
