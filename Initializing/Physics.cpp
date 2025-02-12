#include "Physics.h"
constexpr auto PVD_HOST = "127.0.0.1";

Physics::~Physics()
{
    if (m_Physics != nullptr) m_Physics->release();
    if (m_Foundation != nullptr) m_Foundation->release();
}

void Physics::Setup()
{
    this->CreateFoundationAndPhysics();
}

void Physics::CreateFoundationAndPhysics()
{
    // Physics
    m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_DefaultAllocatorCallback, m_DefaultErrorCallback);
    if (m_Foundation == nullptr)
    {
        throw std::exception("PxCreateFoundation failed!");
    }

    // Create PVD client
    m_Pvd = PxCreatePvd(*m_Foundation);
    m_Transport = physx::PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
    m_Pvd->connect(*m_Transport, physx::PxPvdInstrumentationFlag::eALL);

    // Create physics
    m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, physx::PxTolerancesScale(), true, m_Pvd);
    if (m_Physics == nullptr)
    {
        throw std::exception("PxCreatePhysics failed!");
    }
}