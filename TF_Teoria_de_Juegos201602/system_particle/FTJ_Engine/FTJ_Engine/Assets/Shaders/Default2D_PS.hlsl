struct PS_IN
{
	float4 posW : SV_POSITION;
	float2 UV	: TEXCOORD;
	float4 color : COLOR;
};

texture2D baseTexture : register(t0); // first texture

SamplerState filters[1] : register(s0); // filter 0 using CLAMP

float4 main(PS_IN input) : SV_TARGET
{
	float4	BaseColor = baseTexture.Sample(filters[0], input.UV.xy);
	return BaseColor * input.color;
}