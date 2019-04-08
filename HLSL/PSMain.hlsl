#include "MainHeader.hlsl"

float4 main(SVSOutput Input) : SV_TARGET
{
	return Input.Diffuse;
}