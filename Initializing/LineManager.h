#pragma once

#include <d3d11_1.h>
#include <vector>

// This include is requires for using DirectX smart pointers (ComPtr)
#include <wrl\client.h>
using Microsoft::WRL::ComPtr;

class RenderDevice;
class Physics;
class Scene;

struct LineVertex
{
	// Position
	float x = 0;
	float y = 0;
	float z = 0;

	// Normal
	float normal_x = 0;
	float normal_y = 0;
	float normal_z = 0;

	// Colour
	float r = 0;
	float g = 0;
	float b = 0;
	float a = 0;
};

class LineManager
{
public:
	LineManager(RenderDevice* renderer);
	virtual ~LineManager() = default;

	void Create();

	// Clear line list
	void ClearLines();

	// Add line
	void AddLine(LineVertex a, LineVertex b);

	// Add lines from scene
	void AddSceneLine(Scene* scene);

	// Render lines
	void Render();

private:
	RenderDevice* m_Renderer = nullptr;

	ComPtr<ID3D11Buffer> m_LineVertexBuffer = nullptr;
	std::vector<LineVertex> m_LineList;
};