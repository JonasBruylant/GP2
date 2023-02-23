#include "stdafx.h"
#include "PhysicsXTestScene.h"
#include "CubePosColorNorm.h"

void PhysicsXTestScene::Initialize()
{

	//EnablePhysxDebugRendering(true);


	const auto pPhysX = PhysxManager::GetInstance()->GetPhysics();
	const PxMaterial* pDefaultMaterial = pPhysX->createMaterial(.5f, .5f, 1.f);

	const float cubeSize{ 5.f };
	const XMFLOAT3 actorDimension{ cubeSize, cubeSize, cubeSize };

	m_pCube = new CubePosColorNorm(actorDimension.x, actorDimension.y, actorDimension.z);
	AddGameObject(m_pCube);


	//Cube actor (Makes it so the cube has collision)
	PxRigidDynamic* pCubeActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxBoxGeometry boxGeometry = PxBoxGeometry{actorDimension.x /2.f, actorDimension.y / 2.f, actorDimension.z / 2.f};

	//PxShape* pCubeShape = pPhysX->createShape(boxGeometry, *pDefaultMaterial, true);
	//pCubeActor->attachShape(*pCubeShape);
	//
	//m_pPhysxScene->addActor(*pCubeActor);

	PxRigidActorExt::createExclusiveShape(*pCubeActor, boxGeometry, *pDefaultMaterial);

	//Link CUBE Rigid with CUBE_ACTOR
	m_pCube->AttachRigidActor(pCubeActor);


	//GROUND PLATE
	PxRigidStatic* pPlaneActor = pPhysX->createRigidStatic(PxTransform{ PxQuat{PxPiDivTwo, PxVec3{0,0,1}	} });
	PxRigidActorExt::createExclusiveShape(*pPlaneActor, PxPlaneGeometry{}, *pDefaultMaterial);

	m_pPhysxScene->addActor(*pPlaneActor);
	

}

void PhysicsXTestScene::Update()
{
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, 'R'))
		m_pCube->Translate(0.f, 10.0f, 0.f);
}

void PhysicsXTestScene::Draw() const
{
}

void PhysicsXTestScene::OnSceneActivated()
{
}

void PhysicsXTestScene::OnSceneDeactivated()
{
}
