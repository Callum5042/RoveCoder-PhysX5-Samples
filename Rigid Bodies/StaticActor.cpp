#include "StaticActor.h"
#include "Physics.h"
#include "Scene.h"

StaticActor::StaticActor(Physics* physics, Scene* scene) : m_Physics(physics), m_Scene(scene)
{
}

StaticActor::~StaticActor()
{
	if (m_Actor != nullptr) 
		m_Actor->release();
}

void StaticActor::Create()
{
	physx::PxPhysics* physics = m_Physics->GetPhysics();

	// Create actor at position
	physx::PxVec3 position = physx::PxVec3(physx::PxReal(0.0f), physx::PxReal(2.0f), physx::PxReal(0.0f));
	physx::PxTransform transform(position);
	m_Actor = physics->createRigidDynamic(transform);
	physx::PxRigidBodyExt::updateMassAndInertia(*m_Actor, 100.0f);

	// Add shape to actor
	physx::PxMaterial* material = physics->createMaterial(0.4f, 0.4f, 0.4f);
	physx::PxShape* shape = physics->createShape(physx::PxBoxGeometry(1.0f, 1.0f, 1.0f), *material);
	m_Actor->attachShape(*shape);

	// Add actor to the scene
	m_Scene->GetScene()->addActor(*m_Actor);
}

DirectX::XMMATRIX StaticActor::Transform()
{
	auto t = m_Actor->getGlobalPose();

	DirectX::XMMATRIX transform = DirectX::XMMatrixIdentity();
	transform *= DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(t.q.x, t.q.y, t.q.z, t.q.w));
	transform *= DirectX::XMMatrixTranslation(t.p.x, t.p.y, t.p.z);

	return transform;
}