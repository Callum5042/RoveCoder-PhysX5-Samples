#pragma once

#include "Vertex.h"

#include <vector>

#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

// This include is requires for using DirectX smart pointers (ComPtr)
#include <wrl\client.h>
using Microsoft::WRL::ComPtr;

class RenderDevice;

class Mesh
{
	RenderDevice* m_Renderer = nullptr;

public:
	Mesh(RenderDevice* renderer);
	virtual ~Mesh() = default;

	// Create the model
	void CreateCube(float width, float height, float depth);

	// Render the model
	void Render();

private:
	// Number of indices to draw
	std::vector<Vertex> m_Vertices;
	std::vector<UINT> m_Indices;
	UINT m_IndexCount = 0;

	// Vertex buffer
	void CreateVertexBuffer();
	ComPtr<ID3D11Buffer> m_VertexBuffer = nullptr;

	// Index buffer
	void CreateIndexBuffer();
	ComPtr<ID3D11Buffer> m_IndexBuffer = nullptr;
};