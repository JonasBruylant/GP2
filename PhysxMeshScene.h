#pragma once
#include "GameScene.h"
class PhysxMeshScene final : public GameScene
{
public:
	PhysxMeshScene() :GameScene(L"PhysxMeshScene") {}
	~PhysxMeshScene() override = default;
	PhysxMeshScene(const PhysxMeshScene& other) = delete;
	PhysxMeshScene(PhysxMeshScene&& other) noexcept = delete;
	PhysxMeshScene& operator=(const PhysxMeshScene& other) = delete;
	PhysxMeshScene& operator=(PhysxMeshScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;

	

private:

	GameObject* m_pChairConvex = nullptr;
	GameObject* m_pChairTriangle = nullptr;

	PxVec3 m_KinematicPosition{};

};