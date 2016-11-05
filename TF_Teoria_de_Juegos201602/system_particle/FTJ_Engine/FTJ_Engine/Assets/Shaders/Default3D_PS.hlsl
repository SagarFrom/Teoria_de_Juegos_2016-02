struct PS_INPUT
{
	float4 posW : SV_POSITION;
	float4 pos	: POSITION;
	float2 UV	: TEXCOORD;
	float3 Nrms	: NORMALS;
};

struct TDirectionalLightInfo
{
	float4		v4Direction;
	float4		v4Position;
	float4		v4Color;
};

cbuffer DIRECTIONALLIGHTS : register(b0)
{
	TDirectionalLightInfo directionalLights[1];
};

texture2D baseTexture : register(t0); // first texture

SamplerState SampleTypeClamp : register(s0);
SamplerState SampleTypeWrap  : register(s1);

float4 main(PS_INPUT input) : SV_TARGET
{
	float4 AmbientLight = float4(0.9, 0.9, 0.9, 1);
	float4 SurfaceColor = baseTexture.Sample(SampleTypeWrap, input.UV.xy);

	SurfaceColor *= saturate(AmbientLight);
	return SurfaceColor;
}