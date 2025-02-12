#pragma once

#include <iostream>
#include "PxPhysicsAPI.h"

class UserErrorCallback : public physx::PxDefaultErrorCallback
{
public:
	virtual void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line)
	{
		physx::PxDefaultErrorCallback::reportError(code, message, file, line);
		std::cerr << "Error: " << code << " - " << message << '\n';
	}
};

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

	UserErrorCallback m_DefaultErrorCallback;
	physx::PxDefaultAllocator m_DefaultAllocatorCallback;
	void CreateFoundationAndPhysics();
};