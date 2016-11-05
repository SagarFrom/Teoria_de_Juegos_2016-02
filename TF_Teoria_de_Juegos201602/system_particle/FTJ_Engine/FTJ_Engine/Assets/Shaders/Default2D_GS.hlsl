struct GSOutput
{
	float4 posW : SV_POSITION;
	float2 UV	: TEXCOORD;
	float4 color : COLOR;
};

struct GSInput
{
	float4 posW : SV_POSITION;
};

cbuffer INFO : register(b0)
{
	float4 pos;
	float4 color;
	float2 size;
	float2 ignore;
}

[maxvertexcount(4)]
void main(point GSInput input[1], inout TriangleStream< GSOutput > output)
{
	GSOutput verts[4] = {
		float4(0, 0, 0, 1), float2(0, 0), float4(color),
		float4(0, 0, 0, 1), float2(1, 0), float4(color),
		float4(0, 0, 0, 1), float2(0, 1), float4(color),
		float4(0, 0, 0, 1), float2(1, 1), float4(color) };

	// prep quad for rasterization													 
	for (uint i = 0; i < 4; ++i)
	{
		verts[i].posW.x = i % 2 ? pos.x + size.x : pos.x;
		verts[i].posW.y = i < 2 ? pos.y : pos.y - size.y;
		verts[i].posW.z = pos.z;

		// send verts to the rasterizer
		output.Append(verts[i]);
	}
}