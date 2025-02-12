// Vertex input
struct VertexInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float4 colour : COLOUR;
};

// Pixel input structure
struct PixelInput
{
    float4 positionClipSpace : SV_POSITION;
    float3 position : POSITION;
    float3 normal : NORMAL;
    float4 colour : COLOUR;
};

// World constant buffer
cbuffer WorldBuffer : register(b0)
{
    matrix cModelViewProjection;
    matrix cModelInverse;
    float4 cCameraPosition;
}

// Directional light constant buffer
cbuffer DirectionalLightBuffer : register(b1)
{
    float4 cLightDirection;
}