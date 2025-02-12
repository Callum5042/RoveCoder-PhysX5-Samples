#pragma once

#include <d3d11_1.h>

// This include is requires for using DirectX smart pointers (ComPtr)
#include <wrl\client.h>
using Microsoft::WRL::ComPtr;

class RenderDevice;
class Window;

class RenderTarget
{
	RenderDevice* m_RenderDevice = nullptr;
	Window* m_Window = nullptr;

public:
	RenderTarget(RenderDevice* renderer, Window* window);
	virtual ~RenderTarget() = default;

	void Create(int width, int height);

	// Clear the buffers
	void Clear();

	// Display the rendered scene
	void Present();

	// Resizing
	void Resize(int width, int height);

private:
	// Swapchain
	ComPtr<IDXGISwapChain> m_SwapChain = nullptr;
	ComPtr<IDXGISwapChain1> m_SwapChain1 = nullptr;
	void CreateSwapChain(int width, int height);

	// Render target and depth stencil view
	ComPtr<ID3D11RenderTargetView> m_RenderTargetView = nullptr;
	ComPtr<ID3D11DepthStencilView> m_DepthStencilView = nullptr;
	void CreateRenderTargetAndDepthStencilView(int width, int height);

	// Viewport
	void SetViewport(int width, int height);
};