#include "stdafx.h"
#include "AudioTestScene.h"
#include "SpherePosColorNorm.h"

#include "PhysxManager.h"
#include "SpherePosColorNorm.h"
#include "Logger.h"
#include "SoundManager.h"


void AudioTestScene::Initialize()
{
	EnablePhysxDebugRendering(true);

	const auto pPhysX = PhysxManager::GetInstance()->GetPhysics();
	const auto pDefaultMaterial = pPhysX->createMaterial(.5f, .5f, .5f);

//Ground
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

//Input
	const auto pInput = m_SceneContext.GetInput();
	pInput->AddInputAction({ Play2D, InputTriggerState::pressed, VK_SPACE });
	pInput->AddInputAction({ Play3D, InputTriggerState::pressed, 'B'});
	pInput->AddInputAction({ IncreaseVolume, InputTriggerState::pressed, VK_UP});
	pInput->AddInputAction({ DecreaseVolume, InputTriggerState::pressed, VK_DOWN});

	const auto pFmod = SoundManager::GetInstance()->GetSystem();

//SOUND 2D
	FMOD::Sound* pSound2D{ nullptr };
	FMOD_RESULT result = pFmod->createStream("Resources/Sounds/wave.mp3", FMOD_2D | FMOD_LOOP_NORMAL,nullptr, &pSound2D);
	SoundManager::GetInstance()->ErrorCheck(result);

	result = pFmod->playSound(pSound2D, nullptr, true,&m_pChannel2D);
	SoundManager::GetInstance()->ErrorCheck(result);

//SOUND 3D
	FMOD::Sound* pSound3D{ nullptr };
	FMOD_RESULT result = pFmod->createStream("Resources/Sounds/Thrones.np3", 
		FMOD_3D | FMOD_LOOP_NORMAL | FMOD_3D_LINEARROLLOFF, 
		nullptr, &pSound3D);

	SoundManager::GetInstance()->ErrorCheck(result);

	result = pFmod->playSound(pSound3D, nullptr, true, &m_pChannel3D);
	SoundManager::GetInstance()->ErrorCheck(result);

	m_pChannel3D->set3DMinMaxDistance(0.f, 100.f);
	
}

inline FMOD_VECTOR ToFmod(XMFLOAT3 vector) //DirectX
{
	return FMOD_VECTOR(vector.x, vector.y, vector.z);
}

inline FMOD_VECTOR ToFmod(PxVec3 vector) //PhysX
{
	return FMOD_VECTOR(vector.x, vector.y, vector.z);
}


void AudioTestScene::Update()
{

	//Sphere
	const float torqueForce{ 3.f };
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_RIGHT))
	{
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ 0.f, 0.f, -torqueForce });
	}
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_LEFT))
	{
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ 0.f, 0.f, torqueForce });
	}

	//Sound Input
	if (m_SceneContext.GetInput()->IsActionTriggered(Play2D))
	{
		bool isPlaying{false};
		m_pChannel2D->getPaused(&isPlaying);
		m_pChannel2D->setPaused(!isPlaying);	
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(Play3D))
	{
		bool isPlaying{ false };
		m_pChannel3D->getPaused(&isPlaying);
		m_pChannel3D->setPaused(!isPlaying);
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(IncreaseVolume) )
	{
		float volume{};
		m_pChannel2D->getVolume(&volume);

		volume += 0.1f;
		Clamp(volume, 1.f, 0.f);

		m_pChannel2D->setVolume(volume);

		Logger::GetInstance()->LogFormat(LogLevel::Info, L"Volume changed >> %f", volume);

	}
	if (m_SceneContext.GetInput()->IsActionTriggered(DecreaseVolume))
	{
		float volume{};
		m_pChannel2D->getVolume(&volume);

		volume -= 0.1f;
		Clamp(volume, 1.f, 0.f);

		m_pChannel2D->setVolume(volume);

		Logger::GetInstance()->LogFormat(LogLevel::Info, L"Volume changed >> %f", volume);

	}


//3D SOUND ATTRIBUTES
		
	auto pCam = m_SceneContext.GetCamera();
	auto pos = ToFmod(pCam->GetPosition());
	auto forward = ToFmod(pCam->GetForward());
	auto up = ToFmod(pCam->GetUp());
	
	FMOD_VECTOR vel{};
	const float deltaT = m_SceneContext.GetGameTime()->GetElapsed();
	//Save position from previous frame
	vel.x = (pos.x - m_prevCamPos.x) / deltaT;
	vel.y = (pos.y - m_prevCamPos.y) / deltaT;
	vel.z = (pos.z - m_prevCamPos.z) / deltaT;
	
	m_prevCamPos = pos;

	SoundManager::GetInstance()->GetSystem()->set3DListenerAttributes(0, &pos, &vel, &forward, &up);

	auto spherePos = ToFmod(m_pSphere->GetPosition());
	auto sphereVel = ToFmod(m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->getLinearVelocity());
	m_pChannel3D->set3DAttributes(&spherePos, &sphereVel);
}

void AudioTestScene::Draw() const
{
}
