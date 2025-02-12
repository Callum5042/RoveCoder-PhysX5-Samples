#pragma once

#include "PxPhysicsAPI.h"
#include <DirectXMath.h>
using namespace DirectX;

class Physics;
class Scene;

class DynamicActor
{
	Physics* m_Physics = nullptr;
	Scene* m_Scene = nullptr;

public:
	DynamicActor(Physics* physics, Scene* scene);
	virtual ~DynamicActor();

	void Create();

	DirectX::XMMATRIX Transform();

private:
	physx::PxRigidDynamic* m_Actor = nullptr;
};