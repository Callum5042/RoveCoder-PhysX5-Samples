#include "Scene.h"
#include "Physics.h"

Scene::Scene(Physics* physics) : m_Physics(physics)
{
}

void Scene::Setup()
{
	this->CreateScene();

	m_Scene->setVisualizationParameter(physx::PxVisualizationParameter::eSCALE, 1.0f);
	m_Scene->setVisualizationParameter(physx::PxVisualizationParameter::eACTOR_AXES, 2.0f);
	m_Scene->setVisualizationParameter(physx::PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
}

void Scene::Simulate(double delta_time)
{
	m_Scene->simulate(static_cast<physx::PxReal>(delta_time));
	m_Scene->fetchResults(true);
}

void Scene::CreateScene()
{
	// Create CPU dispatcher
	physx::PxDefaultCpuDispatcher* dispatcher = physx::PxDefaultCpuDispatcherCreate(2);

	// Create scene
	physx::PxSceneDesc scene_desc(m_Physics->GetPhysics()->getTolerancesScale());
	scene_desc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
	scene_desc.cpuDispatcher = dispatcher;
	scene_desc.filterShader = physx::PxDefaultSimulationFilterShader;

	m_Scene = m_Physics->GetPhysics()->createScene(scene_desc);
}