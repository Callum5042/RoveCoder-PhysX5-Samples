#pragma once

#include <exception>

// This include is requires for using DirectX smart pointers (ComPtr)
#include <wrl\client.h>
using Microsoft::WRL::ComPtr;

class Application;

namespace DX
{
	// Throw if the function result has failed. For ease of error handling
	inline void Check(HRESULT hr)
	{
#ifdef _DEBUG
		if (FAILED(hr))
		{
			throw std::exception();
		}
#endif
	}
}