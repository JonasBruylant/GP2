#include "stdafx.h"
#include "SceneSelector.h"
#include "SceneManager.h"

//Change this define to activate/deactive the corresponding scenes
// W01 - W02  (#define ...)

//#define W01
#define W02

#ifdef W01
#include "TestScene.h"
#include "PhysicsXTestScene.h"
#include "W1_AssignmentScene.h"
#endif

#ifdef W02
#include "PhysxMeshScene.h"
#include "TriggerTestScene.h"
#include "AudioTestScene.h"
#include "W2_AssignmentScene.h"
#endif

namespace dae
{
	void AddScenes(SceneManager * pSceneManager)
	{

#ifdef W01
		//pSceneManager->AddGameScene(new TestScene());
		//pSceneManager->AddGameScene(new PhysicsXTestScene());
		pSceneManager->AddGameScene(new W1_AssignmentScene());
#endif

#ifdef W02
		//pSceneManager->AddGameScene(new PhysxMeshScene());
		//pSceneManager->AddGameScene(new TriggerTestScene());
		//pSceneManager->AddGameScene(new AudioTestScene);
		pSceneManager->AddGameScene(new W2_AssignmentScene());
#endif

	}
}

