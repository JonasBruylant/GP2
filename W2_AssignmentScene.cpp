#include "stdafx.h"
#include "W2_AssignmentScene.h"
#include "SpherePosColorNorm.h"
#include "CubePosColorNorm.h"
#include "MeshObject.h"
#include "ContentManager.h"
#include "SoundManager.h"




enum class InputIds : int
{
	Left,
	Right
};

void W2_AssignmentScene::Initialize()
{
	EnablePhysxDebugRendering(true);


	const auto pPhysX = PhysxManager::GetInstance()->GetPhysics();
	const PxMaterial* pSphereMaterial = pPhysX->createMaterial(.5f, .95f, .2f);
	auto pDefaultMaterial = pPhysX->createMaterial(.5f, .5f, .9f);
	auto pCubeMaterial = pPhysX->createMaterial(.5f, .5f, .2f);

//SPHERE OBJECT
	const float sphereRadius{ 1.25f };
	const float sphereRadiusHalf{ sphereRadius/2 };
	const int sliceAndStacks{ 16 };
	m_pBall = new SpherePosColorNorm(sphereRadius, sliceAndStacks, sliceAndStacks, XMFLOAT4{ Colors::Azure });
	AddGameObject(m_pBall);
	m_pBall->Translate(0.f, 5.f, 0.f);

	//Second Sphere
	m_pBall2 = new SpherePosColorNorm(sphereRadiusHalf, sliceAndStacks, sliceAndStacks, XMFLOAT4{ Colors::Azure });
	AddGameObject(m_pBall2);
	m_pBall2->Translate(10.f, 20.f, 0.f);

	//Second Sphere
	m_pBall3 = new SpherePosColorNorm(sphereRadiusHalf, sliceAndStacks, sliceAndStacks, XMFLOAT4{ Colors::Azure });
	AddGameObject(m_pBall3);
	m_pBall3->Translate(-10.f, 20.f, 0.f);

//Sphere actor
	PxRigidDynamic* pSphereActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxRigidDynamic* pSphereActor2 = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxRigidDynamic* pSphereActor3 = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	m_pBallRigid = pSphereActor;
	pSphereActor->setMass(10.f);
	pSphereActor2->setMass(10.f);
	pSphereActor3->setMass(10.f);

	PxSphereGeometry sphereGeometry = PxSphereGeometry{ sphereRadius };
	PxSphereGeometry sphereGeometry2 = PxSphereGeometry{ sphereRadiusHalf };

	PxRigidActorExt::createExclusiveShape(*pSphereActor, sphereGeometry, *pSphereMaterial);
	PxRigidActorExt::createExclusiveShape(*pSphereActor2, sphereGeometry2, *pSphereMaterial);
	PxRigidActorExt::createExclusiveShape(*pSphereActor3, sphereGeometry2, *pSphereMaterial);
	m_pBall->AttachRigidActor(pSphereActor);
	m_pBall2->AttachRigidActor(pSphereActor2);
	m_pBall3->AttachRigidActor(pSphereActor3);

//D6JOINT
	auto d6Joint = PxD6JointCreate(*pPhysX, nullptr, PxTransform{ PxIdentity }, pSphereActor, PxTransform{PxIdentity});
	d6Joint->setMotion(PxD6Axis::eX, PxD6Motion::eFREE);
	d6Joint->setMotion(PxD6Axis::eY, PxD6Motion::eFREE);
	d6Joint->setMotion(PxD6Axis::eTWIST, PxD6Motion::eFREE);
	d6Joint->setMotion(PxD6Axis::eSWING1, PxD6Motion::eFREE);
	d6Joint->setMotion(PxD6Axis::eSWING2, PxD6Motion::eFREE);

//Cubes
	float cubeSize{ 1.5f };
	m_pCube1 = new CubePosColorNorm(cubeSize, cubeSize, cubeSize, XMFLOAT4{ Colors::Red });
	AddGameObject(m_pCube1);
	m_pCube1->Translate(4.f, 5.f, 0.f);

	m_pCube2 = new CubePosColorNorm(cubeSize, cubeSize, cubeSize, XMFLOAT4{ Colors::Blue });
	AddGameObject(m_pCube2);
	m_pCube2->Translate(-4.f, 5.f, 0.f);

	PxRigidDynamic* pCubeActor1 = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxRigidDynamic* pCubeActor2 = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxBoxGeometry boxGeometry = PxBoxGeometry{ cubeSize / 2.f,cubeSize / 2.f, cubeSize / 2.f };
	PxRigidActorExt::createExclusiveShape(*pCubeActor1, boxGeometry, *pCubeMaterial);
	PxRigidActorExt::createExclusiveShape(*pCubeActor2, boxGeometry, *pCubeMaterial);
	m_pCube1->AttachRigidActor(pCubeActor1);
	m_pCube2->AttachRigidActor(pCubeActor2);
	pCubeActor1->setMass(.1f);
	pCubeActor2->setMass(.1f);


	//Hinges
	XMFLOAT3 cubeDimension{3.f, 1.f, 3.f};
	m_pHingeCube1 = new CubePosColorNorm(cubeDimension.x, cubeDimension.y, cubeDimension.z, XMFLOAT4{Colors::Red});
	m_pHingeCube2 = new CubePosColorNorm(cubeDimension.x, cubeDimension.y, cubeDimension.z, XMFLOAT4{Colors::Blue});
	AddGameObject(m_pHingeCube1);
	AddGameObject(m_pHingeCube2);

	m_pHingeCube1->Translate(9.f,17.f, 0.f);
	m_KinematicPositionHinge1 = { m_pHingeCube1->GetPosition().x + cubeDimension.x / 2, 17.f, 0.f};
	m_pHingeCube2->Translate(-9.f, 17.f, 0.f);
	m_KinematicPositionHinge2 = {m_pHingeCube2->GetPosition().x - cubeDimension.x/2, 17.f, 0.f};


	PxRigidDynamic* pCubeActor3 = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxRigidDynamic* pCubeActor4 = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	pCubeActor3->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	pCubeActor4->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

	PxBoxGeometry boxGeometry2 = PxBoxGeometry{ cubeDimension.x/2, cubeDimension.y/2, cubeDimension.z/2 };
	PxRigidActorExt::createExclusiveShape(*pCubeActor3, boxGeometry2, *pCubeMaterial);
	PxRigidActorExt::createExclusiveShape(*pCubeActor4, boxGeometry2, *pCubeMaterial);
	m_pHingeCube1->AttachRigidActor(pCubeActor3);
	m_pHingeCube2->AttachRigidActor(pCubeActor4);
	

//INPUT
	m_SceneContext.GetInput()->AddInputAction(InputAction{
	static_cast<int>(InputIds::Right), InputTriggerState::down, VK_RIGHT, -1, XINPUT_GAMEPAD_DPAD_DOWN });

	m_SceneContext.GetInput()->AddInputAction(InputAction{
	static_cast<int>(InputIds::Left), InputTriggerState::down, VK_LEFT, -1, XINPUT_GAMEPAD_DPAD_DOWN });

//Ground plane
	PxRigidStatic* pPlaneActor = pPhysX->createRigidStatic(PxTransform{ PxQuat{PxPiDivTwo, PxVec3{0,0,1}	} });
	PxRigidActorExt::createExclusiveShape(*pPlaneActor, PxPlaneGeometry{}, *pDefaultMaterial);

	m_pPhysxScene->addActor(*pPlaneActor);


//Level Mesh
//Triangle mesh chair
	m_pLevelTriangle = new MeshObject(L"Resources/Meshes/Level.ovm");
	AddGameObject(m_pLevelTriangle);

	const auto pTriangleMesh = ContentManager::GetInstance()->Load<PxTriangleMesh>(L"Resources/Meshes/Level.ovpt");
	const auto pTriangleActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	pTriangleActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

	PxRigidActorExt::createExclusiveShape(*pTriangleActor, PxTriangleMeshGeometry{ pTriangleMesh }, *pDefaultMaterial);
	m_pLevelTriangle->AttachRigidActor(pTriangleActor);


//Trigger left 
	m_pTriggerLeft = pPhysX->createRigidStatic(PxTransform{ -7.f, 1.f, 0.f });
	auto pShape = PxRigidActorExt::createExclusiveShape(*m_pTriggerLeft, PxBoxGeometry{ 3.f, 1.5f, 3.f }, *pDefaultMaterial);

	pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);

	m_pPhysxScene->addActor(*m_pTriggerLeft);


//Trigger Right
	m_pTriggerRight = pPhysX->createRigidStatic(PxTransform{ 7.f, 1.f, 0.f });
	pShape = PxRigidActorExt::createExclusiveShape(*m_pTriggerRight, PxBoxGeometry{ 3.f, 1.5f, 3.f }, *pDefaultMaterial);

	pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);


	m_pPhysxScene->addActor(*m_pTriggerRight);


//Sound2D

	const auto pFmod = SoundManager::GetInstance()->GetSystem();
	FMOD::Sound* pSound2D{ nullptr };
	FMOD::Sound* pSound2D2{ nullptr };
	FMOD_RESULT result = pFmod->createStream("Resources/Sounds/bell.mp3", FMOD_2D , nullptr, &pSound2D);
	FMOD_RESULT result2 = pFmod->createStream("Resources/Sounds/bell.mp3", FMOD_2D, nullptr, &pSound2D2);
	SoundManager::GetInstance()->ErrorCheck(result);
	SoundManager::GetInstance()->ErrorCheck(result2);

	result = pFmod->playSound(pSound2D, nullptr, true, &m_pChannel2D1);
	result2 = pFmod->playSound(pSound2D2, nullptr, true, &m_pChannel2D2);
	SoundManager::GetInstance()->ErrorCheck(result);
	SoundManager::GetInstance()->ErrorCheck(result2);
}

void W2_AssignmentScene::Update()
{
	const float torqueForce{ 10.f };

	const float movementSpeed{ 20.f * m_SceneContext.GetGameTime()->GetElapsed() };

	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_RIGHT))
	{
		m_pBall->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ 0.f, 0.f, -torqueForce });
	}

	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_LEFT))
	{
		m_pBall->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ 0.f, 0.f, torqueForce });
	}
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, VK_SPACE))
	{
		PxVec3 jumpPower{ 0.f, 2000.f, 0.f };
		m_pBall->GetRigidActor()->is<PxRigidDynamic>()->addForce(jumpPower);
	}

	if (m_isInLeftTrigger)
	{
		m_pHingeCube2->GetRigidActor()->setGlobalPose(PxTransform{ m_KinematicPositionHinge2 });

		if (m_RotationalDegreeLeft > -90)
		{
			m_RotationalDegreeLeft -= movementSpeed;
		}
		m_pHingeCube2->RotateDegrees(0.f, 0.f, m_RotationalDegreeLeft);

	}
	if (m_isInRightTrigger)
	{
		m_pHingeCube1->GetRigidActor()->setGlobalPose(PxTransform{ m_KinematicPositionHinge1 });


		if (m_RotationalDegreeRight < 90)
		{
			m_RotationalDegreeRight += movementSpeed;
		}
		m_pHingeCube1->RotateDegrees(0.f, 0.f, m_RotationalDegreeRight);

	}

}

void W2_AssignmentScene::Draw() const
{
}


void W2_AssignmentScene::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	for (auto i{ 0 }; i < count; ++i)
	{
		//Ignore deleted shapes
		if (pairs[i].flags & (PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER | PxTriggerPairFlag::eREMOVED_SHAPE_OTHER))
			continue;


		const PxTriggerPair& pair = pairs[i];
		if (pair.triggerActor == m_pTriggerLeft && pair.otherActor == m_pCube2->GetRigidActor()) //LEFT TRIGGER
		{
			if (pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND) //ENTER
			{
				Logger::GetInstance()->LogInfo(L"Sphere FOUND triggerbox LEFT");
				bool isPlaying{ true };
				m_pChannel2D1->setPaused(isPlaying);				
				m_pChannel2D1->getPaused(&isPlaying);
				m_pChannel2D1->setPaused(!isPlaying);				
				//m_pHingeCube2->RotateDegrees(0.f, 0.f, 90.f);
				//m_pHingeCube2->Translate(-10.f, 16.f, 0.f);

				m_isInLeftTrigger = true;
			
			}
			else if (pair.status == PxPairFlag::eNOTIFY_TOUCH_LOST) //EXIT
			{

				//Logger::GetInstance()->LogInfo(L"Sphere LOST triggerbox LEFT");
			}
		}
		else if (pair.triggerActor == m_pTriggerRight && pair.otherActor == m_pCube1->GetRigidActor()) //RIGHT TRIGGER
		{
			if (pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND) //ENTER
			{
				Logger::GetInstance()->LogInfo(L"Sphere FOUND triggerbox RIGHT");
				bool isPlaying{ true };
				m_pChannel2D2->setPaused(isPlaying);
				m_pChannel2D2->getPaused(&isPlaying);
				m_pChannel2D2->setPaused(!isPlaying);
				//m_pHingeCube1->RotateDegrees(0.f, 0.f, 90.f);
				//m_pHingeCube1->Translate(10.f, 16.f, 0.f);

				m_isInRightTrigger = true;

			}
			else if (pair.status == PxPairFlag::eNOTIFY_TOUCH_LOST) //EXIT
			{
				//Logger::GetInstance()->LogInfo(L"Sphere LOST triggerbox RIGHT");
			}
		}
	}
}
