#pragma once

#include "PxPhysicsAPI.h"

class Physics
{
public:
	Physics() = default;
	virtual ~Physics();

	void Setup();
	inline physx::PxPhysics* GetPhysics() { return m_Physics; }

private:
	// Setup
	physx::PxPhysics* m_Physics = nullptr;
	physx::PxPvd* m_Pvd = nullptr;
	physx::PxPvdTransport* m_Transport = nullptr;
	physx::PxFoundation* m_Foundation = nullptr;

	physx::PxDefaultErrorCallback m_DefaultErrorCallback;
	physx::PxDefaultAllocator m_DefaultAllocatorCallback;
	void CreateFoundationAndPhysics();
};