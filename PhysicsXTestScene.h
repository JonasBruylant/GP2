#pragma once
#include "GameScene.h"


class PhysicsXTestScene final : public GameScene
{
public:
	PhysicsXTestScene() :GameScene(L"TestScene") {}
	~PhysicsXTestScene() override = default;
	PhysicsXTestScene(const PhysicsXTestScene& other) = delete;
	PhysicsXTestScene(PhysicsXTestScene&& other) noexcept = delete;
	PhysicsXTestScene& operator=(const PhysicsXTestScene& other) = delete;
	PhysicsXTestScene& operator=(PhysicsXTestScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;

private:

	GameObject* m_pCube = nullptr;



};
