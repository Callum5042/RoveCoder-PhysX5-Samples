#include "DynamicActor.h"
#include "Physics.h"
#include "Scene.h"

DynamicActor::DynamicActor(Physics* physics, Scene* scene) : m_Physics(physics), m_Scene(scene)
{
}

DynamicActor::~DynamicActor()
{
	if (m_Actor != nullptr) 
		m_Actor->release();
}

void DynamicActor::Create()
{
	physx::PxPhysics* physics = m_Physics->GetPhysics();

	// Create actor at position
	physx::PxVec3 position = physx::PxVec3(0.0f, 2.0f, 0.0f);
	physx::PxTransform transform(position);
	m_Actor = physics->createRigidDynamic(transform);
	physx::PxRigidBodyExt::updateMassAndInertia(*m_Actor, 100.0f);

	// Add shape to actor
	physx::PxMaterial* material = physics->createMaterial(0.4f, 0.4f, 0.4f);
	physx::PxShape* shape = physics->createShape(physx::PxSphereGeometry(1.0f), *material);
	m_Actor->attachShape(*shape);
	shape->release();

	// Add actor to the scene
	m_Scene->GetScene()->addActor(*m_Actor);
}

void DynamicActor::AddForce(float x, float y, float z)
{
	m_Actor->addForce(physx::PxVec3(x, y, z));
}

DirectX::XMMATRIX DynamicActor::Transform()
{
	auto t = m_Actor->getGlobalPose();

	DirectX::XMMATRIX transform = DirectX::XMMatrixIdentity();
	transform *= DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(t.q.x, t.q.y, t.q.z, t.q.w));
	transform *= DirectX::XMMatrixTranslation(t.p.x, t.p.y, t.p.z);

	return transform;
}