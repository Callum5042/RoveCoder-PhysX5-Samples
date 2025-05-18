#include "RenderTarget.h"
#include "RenderDevice.h"
#include "DxCheck.h"
#include "Window.h"

#include <DirectXColors.h>

RenderTarget::RenderTarget(RenderDevice* renderer, Window* window) : m_RenderDevice(renderer), m_Window(window)
{
}

void RenderTarget::Create(int width, int height)
{
	this->CreateSwapChain(width, height);
	this->CreateRenderTargetAndDepthStencilView(width, height);
	this->SetViewport(width, height);
}

void RenderTarget::CreateSwapChain(int width, int height)
{
	// Query the device until we get the DXGIFactory
	ComPtr<IDXGIDevice> dxgiDevice = nullptr;
	DX::Check(m_RenderDevice->GetDevice()->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(dxgiDevice.GetAddressOf())));

	ComPtr<IDXGIAdapter> adapter = nullptr;
	DX::Check(dxgiDevice->GetAdapter(adapter.GetAddressOf()));

	ComPtr<IDXGIFactory> dxgiFactory = nullptr;
	DX::Check(adapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(dxgiFactory.GetAddressOf())));

	// Query IDXGIFactory to try to get IDXGIFactory2
	ComPtr<IDXGIFactory2> dxgiFactory2 = nullptr;
	DX::Check(dxgiFactory.As(&dxgiFactory2));

	// If we can support IDXGIFactory2 then use it to create the swap chain, otherwise fallback to IDXIFactory
	if (dxgiFactory2 != nullptr)
	{
		// DirectX 11.1
		DXGI_SWAP_CHAIN_DESC1 swapchain_desc = {};
		swapchain_desc.Width = width;
		swapchain_desc.Height = height;
		swapchain_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapchain_desc.SampleDesc.Count = 1;
		swapchain_desc.SampleDesc.Quality = 0;
		swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapchain_desc.BufferCount = 2;
		swapchain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapchain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

		// CreateSwapChainForHwnd is the prefered way of creating the swap chain
		DX::Check(dxgiFactory2->CreateSwapChainForHwnd(m_RenderDevice->GetDevice(), m_Window->GetHwnd(), &swapchain_desc, nullptr, nullptr, &m_SwapChain1));
		DX::Check(m_SwapChain1.As(&m_SwapChain));
	}
	else
	{
		// Describe the swapchain
		DXGI_SWAP_CHAIN_DESC swapchain_desc = {};
		swapchain_desc.BufferDesc.Width = width;
		swapchain_desc.BufferDesc.Height = height;
		swapchain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapchain_desc.SampleDesc.Count = 1;
		swapchain_desc.SampleDesc.Quality = 0;
		swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapchain_desc.BufferCount = 2;
		swapchain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapchain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

		swapchain_desc.BufferDesc.RefreshRate.Numerator = 60;
		swapchain_desc.BufferDesc.RefreshRate.Denominator = 1;
		swapchain_desc.OutputWindow = m_Window->GetHwnd();
		swapchain_desc.Windowed = TRUE;

		// Creates the swapchain
		DX::Check(dxgiFactory->CreateSwapChain(m_RenderDevice->GetDevice(), &swapchain_desc, &m_SwapChain));
	}
}

void RenderTarget::CreateRenderTargetAndDepthStencilView(int width, int height)
{
	// Create the render target view
	ComPtr<ID3D11Texture2D> back_buffer = nullptr;
	DX::Check(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(back_buffer.GetAddressOf())));
	DX::Check(m_RenderDevice->GetDevice()->CreateRenderTargetView(back_buffer.Get(), nullptr, m_RenderTargetView.GetAddressOf()));

	// Describe the depth stencil view
	D3D11_TEXTURE2D_DESC depth_desc = {};
	depth_desc.Width = width;
	depth_desc.Height = height;
	depth_desc.MipLevels = 1;
	depth_desc.ArraySize = 1;
	depth_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depth_desc.SampleDesc.Count = 1;
	depth_desc.SampleDesc.Quality = 0;
	depth_desc.Usage = D3D11_USAGE_DEFAULT;
	depth_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	// Create the depth stencil view
	ComPtr<ID3D11Texture2D> depth_stencil = nullptr;
	DX::Check(m_RenderDevice->GetDevice()->CreateTexture2D(&depth_desc, nullptr, &depth_stencil));
	DX::Check(m_RenderDevice->GetDevice()->CreateDepthStencilView(depth_stencil.Get(), nullptr, m_DepthStencilView.GetAddressOf()));

	// Binds both the render target and depth stencil to the pipeline's output merger stage
	m_RenderDevice->GetDeviceContext()->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), m_DepthStencilView.Get());
}

void RenderTarget::SetViewport(int width, int height)
{
	// Describe the viewport
	D3D11_VIEWPORT viewport = {};
	viewport.Width = static_cast<float>(width);
	viewport.Height = static_cast<float>(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;

	// Bind viewport to the pipline's rasterization stage
	m_RenderDevice->GetDeviceContext()->RSSetViewports(1, &viewport);
}

void RenderTarget::Clear()
{
	// Clear the render target view to the chosen colour
	m_RenderDevice->GetDeviceContext()->ClearRenderTargetView(m_RenderTargetView.Get(), reinterpret_cast<const float*>(&DirectX::Colors::SteelBlue));
	m_RenderDevice->GetDeviceContext()->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Bind the render target view to the pipeline's output merger stage
	m_RenderDevice->GetDeviceContext()->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), m_DepthStencilView.Get());
}

void RenderTarget::Present()
{
	// Use IDXGISwapChain1::Present1 for presenting instead
	// This is a requirement for using variable refresh rate displays
	if (m_SwapChain1 != nullptr)
	{
		BOOL fullscreen;
		DX::Check(m_SwapChain1->GetFullscreenState(&fullscreen, nullptr));

		DXGI_PRESENT_PARAMETERS presentParameters = {};
		UINT present_flags = (fullscreen ? 0 : DXGI_PRESENT_ALLOW_TEARING);
		DX::Check(m_SwapChain1->Present1(0, present_flags, &presentParameters));
	}
	else
	{
		DX::Check(m_SwapChain->Present(0, 0));
	}
}

void RenderTarget::Resize(int width, int height)
{
	// Can only resize if width or height has a positive value to avoid crashing
	if (width <= 0 || height <= 0)
		return;

	// Releases the current render target and depth stencil view
	m_DepthStencilView.ReleaseAndGetAddressOf();
	m_RenderTargetView.ReleaseAndGetAddressOf();

	// Resize the swapchain
	DX::Check(m_SwapChain->ResizeBuffers(2, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING));

	// Creates a new render target and depth stencil view with the new window size
	CreateRenderTargetAndDepthStencilView(width, height);

	// Sets a new viewport with the new window size
	SetViewport(width, height);
}