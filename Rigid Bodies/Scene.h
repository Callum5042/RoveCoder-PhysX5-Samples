#pragma once

#include "PxPhysicsAPI.h"
#include <iostream>

using namespace physx;

class Physics;

class DefaultFilterCallback : public physx::PxSimulationEventCallback
{
public:
	DefaultFilterCallback() = default;

	// Inherited via PxSimulationEventCallback
	void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override
	{
	}

	void onWake(physx::PxActor** actors, physx::PxU32 count) override
	{
	}

	void onSleep(physx::PxActor** actors, physx::PxU32 count) override
	{
	}

	void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override
	{
		std::cout << "Contact made\n";
	}

	void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override
	{
	}

	void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) override
	{
	}
};

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

	DefaultFilterCallback m_FilterCallback;
};