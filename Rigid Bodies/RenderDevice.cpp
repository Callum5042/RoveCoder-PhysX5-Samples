#include "RenderDevice.h"
#include "Application.h"
#include "DxCheck.h"

RenderDevice::RenderDevice()
{
}

void RenderDevice::Create()
{
	// Setup Direct3D 11
	CreateDeviceAndContext();
}

void RenderDevice::CreateDeviceAndContext()
{
	// Look for Direct3D 11 feature
	D3D_FEATURE_LEVEL feature_levels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0
	};

	UINT feature_level_count = ARRAYSIZE(feature_levels);

	// Add debug flag if in debug mode
	D3D11_CREATE_DEVICE_FLAG deviceFlag = (D3D11_CREATE_DEVICE_FLAG)0;
#ifdef _DEBUG
	deviceFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif

	// Create device and device context
	D3D_FEATURE_LEVEL feature_level;
	DX::Check(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, deviceFlag, feature_levels, feature_level_count,
		D3D11_SDK_VERSION, m_Device.ReleaseAndGetAddressOf(), &feature_level, m_DeviceContext.ReleaseAndGetAddressOf()));

	// Check if Direct3D 11.1 is supported, if not fall back to Direct3D 11
	if (feature_level != D3D_FEATURE_LEVEL_11_1)
	{
		MessageBox(NULL, L"Unsupported", L"D3D_FEATURE_LEVEL_11_1 is not supported! Falling back to D3D_FEATURE_LEVEL_11_0", MB_OK);
	}

	// Check if Direct3D 11 is supported
	if (feature_level != D3D_FEATURE_LEVEL_11_1)
	{
		MessageBox(NULL, L"Unsupported", L"D3D_FEATURE_LEVEL_11_0 is not supported", MB_OK);
		throw std::exception();
	}
}