#include "MainHeader.hlsl"

#pragma pack_matrix(row_major)

cbuffer cbMatrix
{
	matrix WVP;
};

SVSOutput main(SVSInput Input)
{
	SVSOutput Output;

	Output.Position = mul(Input.Position, WVP);
	Output.TexCoord = Input.TexCoord;
	Output.Normal = Input.Normal;
	Output.Diffuse = Input.Diffuse;
	Output.Specular = Input.Specular;

	return Output;
}