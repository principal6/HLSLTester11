#pragma once

#include "SharedHeader.h"

struct SMesh
{
	SVertexMainData		VertexData{};
	ID3D11Buffer*		VertexBuffer{};

	SIndex3Data			IndexData{};
	ID3D11Buffer*		IndexBuffer{};

	SMesh() {};
	~SMesh()
	{
		RELEASE_NULLPTR(VertexBuffer);
		RELEASE_NULLPTR(IndexBuffer);
	}
};

class CMeshContainer
{
public:
	auto Create(ID3D11Device* PtrDevice, ID3D11DeviceContext* PtrDeviceContext) noexcept->CMeshContainer&;

	// Vertices must be in clock-wise order
	auto MakeTriangle(XMFLOAT2 XY1, XMFLOAT2 XY2, XMFLOAT2 XY3) noexcept->CMeshContainer&;
	auto MakeCube(float Size) noexcept->CMeshContainer&;
	auto MakeSphere(float Radius, uint8_t VerticalDetail, uint8_t HorizontalDetail, XMFLOAT3 Color) noexcept->CMeshContainer&;

	auto SetMatrix(XMMATRIX World, XMMATRIX ViewProjection) noexcept->CMeshContainer&;
	
	auto GetCBMatrixData() const noexcept { return &m_CBMatrixData; }

	void DrawMesh() noexcept;

private:
	void CreateBuffers() noexcept;

private:
	ID3D11Device*			m_pDevice{};
	ID3D11DeviceContext*	m_pDeviceContext{};
	SMesh					m_Mesh{};
	SCBMatrixData			m_CBMatrixData{};
	bool					m_IsMeshCreated{ false };
};