#pragma once

#include "PxPhysicsAPI.h"
#include <DirectXMath.h>
using namespace DirectX;

class Physics;
class Scene;

class StaticActor
{
	Physics* m_Physics = nullptr;
	Scene* m_Scene = nullptr;

public:
	StaticActor(Physics* physics, Scene* scene);
	virtual ~StaticActor();

	void Create();

	DirectX::XMMATRIX Transform();

private:
	physx::PxRigidDynamic* m_Actor = nullptr;
};