#pragma once

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <memory>
#include <string>

#include "Physics.h"

#include <DirectXMath.h>
using namespace DirectX;

class Window;

class RenderDevice;
class RenderTarget;
class Shader;

class Camera;
class Mesh;

class RasterState;
class TextureSampler;

class Physics;
class Scene;
class LineManager;

class Application
{
public:
	Application();
	virtual ~Application() = default;

	int Execute();

	LRESULT HandleMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// Get window
	inline Window* GetWindow() const { return m_Window.get(); }

private:
	std::unique_ptr<Window> m_Window = nullptr;
	std::unique_ptr<RenderDevice> m_RenderDevice = nullptr;
	std::unique_ptr<RenderTarget> m_RenderTarget = nullptr;
	std::unique_ptr<Shader> m_Shader = nullptr;
	std::unique_ptr<Mesh> m_Mesh = nullptr;
	std::unique_ptr<Camera> m_Camera = nullptr;
	std::unique_ptr<Physics> m_Physics = nullptr;
	std::unique_ptr<Scene> m_Scene = nullptr;

	std::unique_ptr<LineManager> m_LineManager = nullptr;

	bool m_Running = true;
	bool m_WindowCreated = false;
	std::string m_ApplicationTitle = "Initializing";

	// On resized event
	void OnResized(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// On mouse move event
	void OnMouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// Calculate frame stats
	void CalculateFrameStats(float delta_time);
	int m_FrameCount = 0;

	// Compute model view projection of the camera
	void UpdateWorldConstantBuffer(const DirectX::XMMATRIX& world);
};