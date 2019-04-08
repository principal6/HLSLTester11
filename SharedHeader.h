#pragma once

#include <string>
#include <cassert>
#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#include "DirectXTK/pch.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "DirectXTK.lib")

using namespace std;
using namespace DirectX;

#define RELEASE_NULLPTR(obj) { if(obj){ obj->Release(); obj = nullptr; } }

static constexpr D3D11_INPUT_ELEMENT_DESC KInputElementDescriptionMain[] =
{
	{ "POSITION"	, 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD"	, 0, DXGI_FORMAT_R32G32_FLOAT		, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL"		, 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR"		, 0, DXGI_FORMAT_R32G32B32A32_FLOAT	, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // COLOR0 = Diffuse
	{ "COLOR"		, 1, DXGI_FORMAT_R32G32B32A32_FLOAT	, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // COLOR1 = Specular
};

struct SPosition
{
	uint32_t	X{};
	uint32_t	Y{};

	constexpr SPosition() {};
	constexpr SPosition(uint32_t _X, uint32_t _Y) : X{ _X }, Y{ _Y } {};
};

struct SSize
{
	uint32_t	X{};
	uint32_t	Y{};

	constexpr SSize() {};
	constexpr SSize(uint32_t _X, uint32_t _Y) : X{ _X }, Y{ _Y } {};
};

struct SVertexMain
{
	XMFLOAT3	Position{};
	XMFLOAT2	TexCoord{};
	XMFLOAT3	Normal{};
	XMFLOAT4	Diffuse{};
	XMFLOAT4	Specular{};

	SVertexMain() {};
	SVertexMain(float x, float y, float z, float u, float v, float cr, float cg, float cb, float ca)
		: Position{ XMFLOAT3(x, y, z) }, TexCoord{ XMFLOAT2(u, v) }, Diffuse{ XMFLOAT4(cr, cg, cb, ca) } {};
};

struct SVertexMainData
{
	vector<SVertexMain>	vVertices{};
	uint32_t			Stride{ static_cast<uint32_t>(sizeof(SVertexMain)) };
	uint32_t			Offset{};

	auto GetCount() const noexcept { return static_cast<uint32_t>(vVertices.size()); }
	auto GetByteSize() const noexcept { return Stride * GetCount(); }
	auto GetPtrData() const noexcept { return &vVertices[0]; }
	auto GetPtrStride() const noexcept { return &Stride; }
	auto GetPtrOffset() const noexcept { return &Offset; }
};

struct SIndex3
{
	DWORD	_0{};
	DWORD	_1{};
	DWORD	_2{};

	SIndex3() {};
	SIndex3(DWORD __0, DWORD __1, DWORD __2) : _0{ __0 }, _1{ __1 }, _2{ __2 } {};
};

struct SIndex3Data
{
	vector<SIndex3>	vIndices{};

	auto GetCount() const noexcept { return static_cast<uint32_t>(vIndices.size() * 3); }
	auto GetByteSize() const noexcept { return static_cast<uint32_t>(sizeof(SIndex3)) * GetCount(); }
	auto GetPtrData() const noexcept { return &vIndices[0]; }
};

struct SCBMatrixData
{
	XMMATRIX	WVP{};
};