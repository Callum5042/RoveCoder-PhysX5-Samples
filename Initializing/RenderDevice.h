#pragma once

#include <d3d11_1.h>

// This include is requires for using DirectX smart pointers (ComPtr)
#include <wrl\client.h>
using Microsoft::WRL::ComPtr;

class RenderDevice
{
public:
	RenderDevice();
	virtual ~RenderDevice() = default;

	// Creates the rendering device and context
	void Create();

	// Get render device
	inline ID3D11Device* GetDevice() const { return m_Device.Get(); }

	// Get render context
	inline ID3D11DeviceContext* GetDeviceContext() const { return m_DeviceContext.Get(); }

private:
	// Device and device context
	ComPtr<ID3D11Device> m_Device = nullptr;
	ComPtr<ID3D11DeviceContext> m_DeviceContext = nullptr;
	void CreateDeviceAndContext();
};