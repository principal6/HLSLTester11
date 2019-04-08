struct SVSInput
{
	float4 Position	: POSITION;
	float2 TexCoord	: TEXCOORD;
	float3 Normal	: NORMAL;
	float4 Diffuse	: COLOR0;
	float4 Specular	: COLOR1;
};

struct SVSOutput
{
	float4 Position	: SV_POSITION;
	float2 TexCoord	: TEXCOORD;
	float3 Normal	: NORMAL;
	float4 Diffuse	: COLOR0;
	float4 Specular	: COLOR1;
};