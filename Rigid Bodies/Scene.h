#pragma once

#include "PxPhysicsAPI.h"

class Physics;

class Scene
{
	Physics* m_Physics = nullptr;

public:
	Scene(Physics* physics);
	virtual ~Scene() = default;

	void Setup();
	void Simulate(double delta_time);

	inline physx::PxScene* GetScene() { return m_Scene; }

private:
	physx::PxScene* m_Scene = nullptr;
	void CreateScene();
};