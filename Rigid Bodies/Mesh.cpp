#include "Mesh.h"
#include "RenderDevice.h"
#include "DxCheck.h"

Mesh::Mesh(RenderDevice* renderer) : m_Renderer(renderer)
{
}

void Mesh::CreateCube(float width, float height, float depth)
{
	float w2 = width;
	float h2 = height;
	float d2 = depth;

	// Vertices
	m_Vertices.resize(24);
	m_Vertices =
	{
		{ VertexPosition(-width, -height, -depth), VertexNormal(+0.0f, +0.0f, -1.0f) },
		{ VertexPosition(-width, +height, -depth), VertexNormal(+0.0f, +0.0f, -1.0f) },
		{ VertexPosition(+width, +height, -depth), VertexNormal(+0.0f, +0.0f, -1.0f) },
		{ VertexPosition(+width, -height, -depth), VertexNormal(+0.0f, +0.0f, -1.0f) },

		{ VertexPosition(-width, -height, +depth), VertexNormal(+0.0f, +0.0f, +1.0f) },
		{ VertexPosition(+width, -height, +depth), VertexNormal(+0.0f, +0.0f, +1.0f) },
		{ VertexPosition(+width, +height, +depth), VertexNormal(+0.0f, +0.0f, +1.0f) },
		{ VertexPosition(-width, +height, +depth), VertexNormal(+0.0f, +0.0f, +1.0f) },

		{ VertexPosition(-width, +height, -depth), VertexNormal(+0.0f, +1.0f, +0.0f) },
		{ VertexPosition(-width, +height, +depth), VertexNormal(+0.0f, +1.0f, +0.0f) },
		{ VertexPosition(+width, +height, +depth), VertexNormal(+0.0f, +1.0f, +0.0f) },
		{ VertexPosition(+width, +height, -depth), VertexNormal(+0.0f, +1.0f, +0.0f) },

		{ VertexPosition(-width, -height, -depth), VertexNormal(+0.0f, -1.0f, +0.0f) },
		{ VertexPosition(+width, -height, -depth), VertexNormal(+0.0f, -1.0f, +0.0f) },
		{ VertexPosition(+width, -height, +depth), VertexNormal(+0.0f, -1.0f, +0.0f) },
		{ VertexPosition(-width, -height, +depth), VertexNormal(+0.0f, -1.0f, +0.0f) },

		{ VertexPosition(-width, -height, +depth), VertexNormal(-1.0f, +0.0f, +0.0f) },
		{ VertexPosition(-width, +height, +depth), VertexNormal(-1.0f, +0.0f, +0.0f) },
		{ VertexPosition(-width, +height, -depth), VertexNormal(-1.0f, +0.0f, +0.0f) },
		{ VertexPosition(-width, -height, -depth), VertexNormal(-1.0f, +0.0f, +0.0f) },

		{ VertexPosition(+width, -height, -depth), VertexNormal(+1.0f, +0.0f, +0.0f) },
		{ VertexPosition(+width, +height, -depth), VertexNormal(+1.0f, +0.0f, +0.0f) },
		{ VertexPosition(+width, +height, +depth), VertexNormal(+1.0f, +0.0f, +0.0f) },
		{ VertexPosition(+width, -height, +depth), VertexNormal(+1.0f, +0.0f, +0.0f) }
	};

	// Indices
	m_Indices.resize(36);
	m_Indices =
	{
		0, 1, 2,
		0, 2, 3,
		4, 5, 6,
		4, 6, 7,
		8, 9, 10,
		8, 10, 11,
		12, 13, 14,
		12, 14, 15,
		16, 17, 18,
		16, 18, 19,
		20, 21, 22,
		20, 22, 23,
	};

	m_IndexCount = static_cast<UINT>(m_Indices.size());

	// Build buffer
	this->CreateVertexBuffer();
	this->CreateIndexBuffer();
}

void Mesh::CreateVertexBuffer()
{
	ID3D11Device* device = m_Renderer->GetDevice();

	// Create vertex buffer
	D3D11_BUFFER_DESC vertexbuffer_desc = {};
	vertexbuffer_desc.Usage = D3D11_USAGE_DEFAULT;
	vertexbuffer_desc.ByteWidth = static_cast<UINT>(sizeof(Vertex) * m_Vertices.size());
	vertexbuffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertex_subdata = {};
	vertex_subdata.pSysMem = m_Vertices.data();

	DX::Check(device->CreateBuffer(&vertexbuffer_desc, &vertex_subdata, m_VertexBuffer.ReleaseAndGetAddressOf()));
}

void Mesh::CreateIndexBuffer()
{
	ID3D11Device* device = m_Renderer->GetDevice();

	// Create index buffer
	D3D11_BUFFER_DESC index_buffer_desc = {};
	index_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	index_buffer_desc.ByteWidth = static_cast<UINT>(sizeof(UINT) * m_Indices.size());
	index_buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA index_subdata = {};
	index_subdata.pSysMem = m_Indices.data();

	DX::Check(device->CreateBuffer(&index_buffer_desc, &index_subdata, m_IndexBuffer.ReleaseAndGetAddressOf()));
}

void Mesh::Render()
{
	ID3D11DeviceContext* context = m_Renderer->GetDeviceContext();

	// We need to define the stride and offset
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	// Bind the vertex buffer to the pipeline's Input Assembler stage
	context->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);

	// Bind the index buffer to the pipeline's Input Assembler stage
	context->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	// Bind the geometry topology to the pipeline's Input Assembler stage
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Render geometry
	context->DrawIndexed(m_IndexCount, 0, 0);
}