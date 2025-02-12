#pragma once

#include "PxPhysicsAPI.h"

class Physics;
class Scene;

class StaticFloorActor
{
	Physics* m_Physics = nullptr;
	Scene* m_Scene = nullptr;

public:
	StaticFloorActor(Physics* physics, Scene* scene);
	virtual ~StaticFloorActor();

	void Create();

private:
	physx::PxRigidStatic* m_RigidBody = nullptr;
};