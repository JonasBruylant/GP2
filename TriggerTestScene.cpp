#include "stdafx.h"
#include "TriggerTestScene.h"
#include "PhysxManager.h"
#include "SpherePosColorNorm.h"
#include "Logger.h"


void TriggerTestScene::Initialize()
{
	EnablePhysxDebugRendering(true);

	const auto pPhysX = PhysxManager::GetInstance()->GetPhysics();
	const auto pDefaultMaterial = pPhysX->createMaterial(.5f, .5f, .5f);


	PxRigidStatic* pGroundActor = pPhysX->createRigidStatic(PxTransform{ PxQuat{PxPiDivTwo, PxVec3{0,0,1}	} });

	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, *pDefaultMaterial);
	m_pPhysxScene->addActor(*pGroundActor);


	//Sphere
	m_pSphere = new SpherePosColorNorm(1.f, 10, 10, XMFLOAT4{ Colors::Gray });
	AddGameObject(m_pSphere);

	const auto pSphereActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxRigidActorExt::createExclusiveShape(*pSphereActor, PxSphereGeometry{ 1.f }, *pDefaultMaterial);
	m_pSphere->AttachRigidActor(pSphereActor);

	m_pSphere->Translate(0.f, 2.f, 0.f);

	//Trigger left 
	m_pTriggerLeft = pPhysX->createRigidStatic(PxTransform{-8.f, .5f, 0.f});
	auto pShape = PxRigidActorExt::createExclusiveShape(*m_pTriggerLeft, PxBoxGeometry{.5f, 10.f, .5f}, *pDefaultMaterial);
	
	pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
	
	m_pPhysxScene->addActor(*m_pTriggerLeft);


	//Trigger Right
	m_pTriggerRight = pPhysX->createRigidStatic(PxTransform{ 8.f, .5f, 0.f });
	pShape = PxRigidActorExt::createExclusiveShape(*m_pTriggerRight, PxBoxGeometry{ .5f, 10.f, .5f }, *pDefaultMaterial);

	pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);


	m_pPhysxScene->addActor(*m_pTriggerRight);

}

void TriggerTestScene::Update()
{

	const float torqueForce{ 3.f };
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_RIGHT))
	{
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ 0.f, 0.f, -torqueForce });
	}

	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_LEFT))
	{
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ 0.f, 0.f, torqueForce });
	}

	const float force{9.f};
	if (m_IsTriggeredLeft)
	{
		m_IsTriggeredLeft = false;
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addForce({ force, force, 0 }, PxForceMode::eIMPULSE);
	}
	if (m_IsTriggerRight)
	{
		m_IsTriggerRight = false;
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addForce({ -force, force, 0 }, PxForceMode::eIMPULSE);
	}
}

void TriggerTestScene::Draw() const
{
}



void TriggerTestScene::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	for (auto i{ 0 }; i < count; ++i)
	{
		//Ignore deleted shapes
		if (pairs[i].flags & (PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER | PxTriggerPairFlag::eREMOVED_SHAPE_OTHER))
			continue;


		const PxTriggerPair& pair = pairs[i];
		if (pair.triggerActor == m_pTriggerLeft) //LEFT TRIGGER
		{
			if (pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND) //ENTER
			{
				Logger::GetInstance()->LogInfo(L"Sphere FOUND triggerbox LEFT");
				m_IsTriggeredLeft = true;
			}
			else if (pair.status == PxPairFlag::eNOTIFY_TOUCH_LOST) //EXIT
			{

				Logger::GetInstance()->LogInfo(L"Sphere LOST triggerbox LEFT");
			}
		}
		else if (pair.triggerActor == m_pTriggerRight) //RIGHT TRIGGER
		{
			if (pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND) //ENTER
			{
				Logger::GetInstance()->LogInfo(L"Sphere FOUND triggerbox RIGHT");
				m_IsTriggerRight = true;
			}
			else if (pair.status == PxPairFlag::eNOTIFY_TOUCH_LOST) //EXIT
			{

				Logger::GetInstance()->LogInfo(L"Sphere LOST triggerbox RIGHT");
			}
		}
	}
}
