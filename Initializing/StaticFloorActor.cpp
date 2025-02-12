#include "StaticFloorActor.h"
#include "Physics.h"
#include "Scene.h"

StaticFloorActor::StaticFloorActor(Physics* physics, Scene* scene) : m_Physics(physics), m_Scene(scene)
{
}

StaticFloorActor::~StaticFloorActor()
{
	if (m_RigidBody != nullptr) m_RigidBody->release();
}

void StaticFloorActor::Create()
{
	// Creates the rigid body
	physx::PxPlane plane(physx::PxPlane(physx::PxVec3(0.0f, 1.0f, 0.0f), 1.0f));
	physx::PxTransform transform = physx::PxTransformFromPlaneEquation(plane);
	m_RigidBody = m_Physics->GetPhysics()->createRigidStatic(transform);

	// Attaches the plane shape to the body
	physx::PxMaterial* material = m_Physics->GetPhysics()->createMaterial(0.4f, 0.4f, 0.4f);
	physx::PxShape* shape = m_Physics->GetPhysics()->createShape(physx::PxPlaneGeometry(), &material, 1, true);
	m_RigidBody->attachShape(*shape);

	// Add actor to the scene for simulation
	m_Scene->GetScene()->addActor(*m_RigidBody);
}