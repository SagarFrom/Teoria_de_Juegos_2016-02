#pragma pack_matrix(row_major)

struct VSInput
{
	float3 posL : POSITION;
	float3 UVs	: TEXCOORD;
	float3 Nrms	: NORMALS;
};

struct VSOutput
{
	float4 posW : SV_POSITION;
	float4 pos	: POSITION;
	float2 UV	: TEXCOORD;
	float3 Nrms	: NORMALS;

};

cbuffer OBJECT : register(b0)
{
	float4x4 worldMatrix;
}
cbuffer SCENE : register(b1)
{
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
}

VSOutput main(VSInput input)
{
	VSOutput output = (VSOutput)0;
	float4 localW = float4(input.posL, 1);
	// Move local space vertex from vertex buffer into world space.
	localW = mul(localW, worldMatrix);
	output.pos = localW;

	// Move into view space, then projection space
	localW = mul(localW, viewMatrix);
	localW = mul(localW, projectionMatrix);

	output.posW = localW;
	output.UV = input.UVs.xy;
	output.Nrms = mul(float4(input.Nrms,0), worldMatrix).xyz;

	return output;
}