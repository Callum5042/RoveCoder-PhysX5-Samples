#include "Application.h"
#include "Window.h"
#include "Timer.h"
#include "RenderDevice.h"
#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"
#include "Physics.h"
#include "Scene.h"
#include "RenderTarget.h"
#include "LineManager.h"

#include "StaticFloorActor.h"

#include <DirectXMath.h>
using namespace DirectX;

#include <windowsx.h>

Application::Application()
{
	const int window_width = 800;
	const int window_height = 600;

	// Create window
	m_Window = std::make_unique<Window>(this);
	m_WindowCreated = m_Window->Create(m_ApplicationTitle, window_width, window_height, false);

	// Create renderer
	m_Renderer = std::make_unique<RenderDevice>();
	m_Renderer->Create();

	m_RenderTarget = std::make_unique<RenderTarget>(m_Renderer.get(), m_Window.get());
	m_RenderTarget->Create(window_width, window_height);

	// Create shader
	m_Shader = std::make_unique<Shader>(m_Renderer.get());
	m_Shader->Load();

	m_Shader->UpdateDirectionalLightBuffer(DirectX::XMFLOAT4(0.7f, -0.6f, 0.4f, 1.0f));

	// Create camera
	m_Camera = std::make_unique<Camera>(window_width, window_height);

	// Physics
	m_Physics = std::make_unique<Physics>();
	m_Physics->Setup();

	m_Scene = std::make_unique<Scene>(m_Physics.get());
	m_Scene->Setup();

	m_LineManager = std::make_unique<LineManager>(m_Renderer.get());
	m_LineManager->Create();
}

int Application::Execute()
{
	Timer timer;
	timer.Start();

	// Model
	m_Mesh = std::make_unique<Mesh>(m_Renderer.get());
	m_Mesh->Create();

	CreatePhysicsActor();

	std::unique_ptr<StaticFloorActor> m_FloorActor = std::make_unique<StaticFloorActor>(m_Physics.get(), m_Scene.get());
	m_FloorActor->Create();

	// Main application loop
	while (m_Running)
	{
		timer.Tick();
		this->CalculateFrameStats(timer.DeltaTime());

		MSG msg = {};
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				m_Running = false;
			}

			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
		else
		{
			m_Scene->Simulate(timer.DeltaTime());

			// Clear the buffers
			m_RenderTarget->Clear();

			// Bind the shader to the pipeline
			m_Shader->Use();

			// Render the model
			auto t = m_Body->getGlobalPose();
			m_Mesh->World = DirectX::XMMatrixTranslation(t.p.x, t.p.y, t.p.z);

			this->UpdateWorldConstantBuffer(m_Mesh->World);
			m_Mesh->Render();

			// Lines
			m_LineManager->ClearLines();
			m_LineManager->AddSceneLine(m_Scene.get());
			this->UpdateWorldConstantBuffer(DirectX::XMMatrixIdentity());
			m_LineManager->Render();

			// Display the rendered scene
			m_RenderTarget->Present();
		}
	}

	return 0;
}

LRESULT Application::HandleMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

		case WM_SIZE:
			this->OnResized(hwnd, msg, wParam, lParam);
			return 0;

		case WM_MOUSEMOVE:
			this->OnMouseMove(hwnd, msg, wParam, lParam);
			return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void Application::OnResized(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Window resized is called upon window creation, so ignore if the window has not finished being created
	if (!m_WindowCreated)
		return;

	// Get window size
	int window_width = LOWORD(lParam);
	int window_height = HIWORD(lParam);

	// Resize renderer
	m_RenderTarget->Resize(window_width, window_height);

	// Update camera
	m_Camera->UpdateAspectRatio(window_width, window_height);
}

void Application::OnMouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static int previous_mouse_x = 0;
	static int previous_mouse_y = 0;

	int mouse_x = static_cast<int>(GET_X_LPARAM(lParam));
	int mouse_y = static_cast<int>(GET_Y_LPARAM(lParam));

	if (wParam & MK_LBUTTON)
	{
		float relative_mouse_x = static_cast<float>(mouse_x - previous_mouse_x);
		float relative_mouse_y = static_cast<float>(mouse_y - previous_mouse_y);

		// Rotate camera
		float yaw = relative_mouse_x * 0.01f;
		float pitch = relative_mouse_y * 0.01f;

		m_Camera->Rotate(pitch, yaw);
	}

	previous_mouse_x = mouse_x;
	previous_mouse_y = mouse_y;
}

void Application::CalculateFrameStats(float delta_time)
{
	static float time = 0.0f;

	m_FrameCount++;
	time += delta_time;

	// Update window title every second with FPS
	if (time > 1.0f)
	{
		std::string frame_title = "(FPS: " + std::to_string(m_FrameCount) + ")";
		m_Window->SetTitle(m_ApplicationTitle + " " + frame_title);

		time = 0.0f;
		m_FrameCount = 0;
	}
}

void Application::UpdateWorldConstantBuffer(const DirectX::XMMATRIX& world)
{
	// View Projection
	DirectX::XMMATRIX matrix = DirectX::XMMatrixIdentity();
	matrix *= world;
	matrix *= m_Camera->GetView();
	matrix *= m_Camera->GetProjection();

	DirectX::XMFLOAT3 position = m_Camera->GetPosition();
	DirectX::XMMATRIX inverse_model = DirectX::XMMatrixInverse(nullptr, world);
	m_Shader->UpdateModelViewProjectionBuffer(matrix, inverse_model, position);
}

void Application::CreatePhysicsActor()
{
	physx::PxMaterial* material = m_Physics->GetPhysics()->createMaterial(0.4f, 0.4f, 0.4f);
	physx::PxShape* shape = m_Physics->GetPhysics()->createShape(physx::PxBoxGeometry(1.0f, 1.0f, 1.0f), *material);

	// Set position
	physx::PxVec3 position = physx::PxVec3(physx::PxReal(0.0f), physx::PxReal(2.0f), physx::PxReal(0.0f));
	physx::PxTransform transform(position);

	m_Body = m_Physics->GetPhysics()->createRigidDynamic(transform);
	m_Body->attachShape(*shape);
	physx::PxRigidBodyExt::updateMassAndInertia(*m_Body, 100.0f);
	m_Scene->GetScene()->addActor(*m_Body);
}