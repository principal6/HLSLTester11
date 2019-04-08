#include "CMeshContainer.h"

auto CMeshContainer::Create(ID3D11Device* PtrDevice, ID3D11DeviceContext* PtrDeviceContext) noexcept->CMeshContainer&
{
	m_pDevice = PtrDevice;
	m_pDeviceContext = PtrDeviceContext;

	return *this;
}

auto CMeshContainer::MakeTriangle(XMFLOAT2 XY1, XMFLOAT2 XY2, XMFLOAT2 XY3) noexcept->CMeshContainer&
{
	if (m_IsMeshCreated)
	{
		return *this;
	}

	XMFLOAT3 Color[3]
		= { {1, 0, 0}, {0, 1, 0}, {0, 0, 1}, };

	/*
	** Vertex
	*/
	SVertexMainData	vert_data;
	// Up (LeftUp - RightUp - LeftDown - RightDown order)
	vert_data.vVertices.push_back(SVertexMain(XY1.x, XY1.y, 0, 0, 0, Color[0].x, Color[0].y, Color[0].z, 1));
	vert_data.vVertices.push_back(SVertexMain(XY2.x, XY2.y, 0, 1, 0, Color[1].x, Color[1].y, Color[1].z, 1));
	vert_data.vVertices.push_back(SVertexMain(XY3.x, XY3.y, 0, 0, 1, Color[2].x, Color[2].y, Color[2].z, 1));

	/*
	** Index
	*/
	SIndex3Data ind_data;
	for (uint32_t i = 0; i < vert_data.GetCount() / 3; ++i)
	{
		// Clock-wise winding
		ind_data.vIndices.push_back(SIndex3(i * 3, i * 3 + 1, i * 3 + 2));
	}

	// Save data
	m_Mesh.VertexData = vert_data;
	m_Mesh.IndexData = ind_data;

	// Create buffers
	CreateBuffers();

	m_IsMeshCreated = true;
	
	return *this;
}

auto CMeshContainer::MakeCube(float Size) noexcept->CMeshContainer&
{
	if (m_IsMeshCreated)
	{
		return *this;
	}

	float half_size = Size / 2.0f;

	XMFLOAT3 UpColor[4]
		= { {1, 1, 1}, {1, 0, 0}, {0, 1, 0}, {0, 0, 1} };

	XMFLOAT3 DownColor[4]
		= { {1, 1, 0}, {0, 1, 1}, {1, 0, 1}, {0, 0, 0} };

	/*
	** Vertex
	*/
	SVertexMainData	vert_data;
	// Up (LeftUp - RightUp - LeftDown - RightDown order)
	vert_data.vVertices.push_back(SVertexMain(-half_size, +half_size, +half_size, 0, 0, UpColor[0].x, UpColor[0].y, UpColor[0].z, 1));
	vert_data.vVertices.push_back(SVertexMain(+half_size, +half_size, +half_size, 1, 0, UpColor[1].x, UpColor[1].y, UpColor[1].z, 1));
	vert_data.vVertices.push_back(SVertexMain(-half_size, +half_size, -half_size, 0, 1, UpColor[2].x, UpColor[2].y, UpColor[2].z, 1));
	vert_data.vVertices.push_back(SVertexMain(+half_size, +half_size, -half_size, 1, 1, UpColor[3].x, UpColor[3].y, UpColor[3].z, 1));

	// Down
	vert_data.vVertices.push_back(SVertexMain(-half_size, -half_size, -half_size, 0, 0, DownColor[0].x, DownColor[0].y, DownColor[0].z, 1));
	vert_data.vVertices.push_back(SVertexMain(+half_size, -half_size, -half_size, 1, 0, DownColor[1].x, DownColor[1].y, DownColor[1].z, 1));
	vert_data.vVertices.push_back(SVertexMain(-half_size, -half_size, +half_size, 0, 1, DownColor[2].x, DownColor[2].y, DownColor[2].z, 1));
	vert_data.vVertices.push_back(SVertexMain(+half_size, -half_size, +half_size, 1, 1, DownColor[3].x, DownColor[3].y, DownColor[3].z, 1));

	// Front
	vert_data.vVertices.push_back(SVertexMain(-half_size, +half_size, -half_size, 0, 0, UpColor[2].x, UpColor[2].y, UpColor[2].z, 1));
	vert_data.vVertices.push_back(SVertexMain(+half_size, +half_size, -half_size, 1, 0, UpColor[3].x, UpColor[3].y, UpColor[3].z, 1));
	vert_data.vVertices.push_back(SVertexMain(-half_size, -half_size, -half_size, 0, 1, DownColor[0].x, DownColor[0].y, DownColor[0].z, 1));
	vert_data.vVertices.push_back(SVertexMain(+half_size, -half_size, -half_size, 1, 1, DownColor[1].x, DownColor[1].y, DownColor[1].z, 1));

	// Right
	vert_data.vVertices.push_back(SVertexMain(+half_size, +half_size, -half_size, 0, 0, UpColor[3].x, UpColor[3].y, UpColor[3].z, 1));
	vert_data.vVertices.push_back(SVertexMain(+half_size, +half_size, +half_size, 1, 0, UpColor[1].x, UpColor[1].y, UpColor[1].z, 1));
	vert_data.vVertices.push_back(SVertexMain(+half_size, -half_size, -half_size, 0, 1, DownColor[1].x, DownColor[1].y, DownColor[1].z, 1));
	vert_data.vVertices.push_back(SVertexMain(+half_size, -half_size, +half_size, 1, 1, DownColor[3].x, DownColor[3].y, DownColor[3].z, 1));

	// Back
	vert_data.vVertices.push_back(SVertexMain(+half_size, +half_size, +half_size, 0, 0, UpColor[1].x, UpColor[1].y, UpColor[1].z, 1));
	vert_data.vVertices.push_back(SVertexMain(-half_size, +half_size, +half_size, 1, 0, UpColor[0].x, UpColor[0].y, UpColor[0].z, 1));
	vert_data.vVertices.push_back(SVertexMain(+half_size, -half_size, +half_size, 0, 1, DownColor[3].x, DownColor[3].y, DownColor[3].z, 1));
	vert_data.vVertices.push_back(SVertexMain(-half_size, -half_size, +half_size, 1, 1, DownColor[2].x, DownColor[2].y, DownColor[2].z, 1));

	// Left
	vert_data.vVertices.push_back(SVertexMain(-half_size, +half_size, +half_size, 0, 0, UpColor[0].x, UpColor[0].y, UpColor[0].z, 1));
	vert_data.vVertices.push_back(SVertexMain(-half_size, +half_size, -half_size, 1, 0, UpColor[2].x, UpColor[2].y, UpColor[2].z, 1));
	vert_data.vVertices.push_back(SVertexMain(-half_size, -half_size, +half_size, 0, 1, DownColor[2].x, DownColor[2].y, DownColor[2].z, 1));
	vert_data.vVertices.push_back(SVertexMain(-half_size, -half_size, -half_size, 1, 1, DownColor[0].x, DownColor[0].y, DownColor[0].z, 1));

	/*
	** Index
	*/
	SIndex3Data ind_data;
	for (uint32_t i = 0; i < vert_data.GetCount() / 4; ++i)
	{
		// Clock-wise winding
		ind_data.vIndices.push_back(SIndex3(i * 4, i * 4 + 1, i * 4 + 2));
		ind_data.vIndices.push_back(SIndex3(i * 4 + 1, i * 4 + 3, i * 4 + 2));
	}

	// Save data
	m_Mesh.VertexData = vert_data;
	m_Mesh.IndexData = ind_data;

	// Create buffers
	CreateBuffers();

	m_IsMeshCreated = true;

	return *this;
}


auto CMeshContainer::MakeSphere(float Radius, uint8_t VerticalDetail, uint8_t HorizontalDetail, XMFLOAT3 Color) noexcept->CMeshContainer&
{
	VerticalDetail = max(VerticalDetail, 4);
	HorizontalDetail = max(HorizontalDetail, 1);
	HorizontalDetail = (HorizontalDetail + 1) * 2;

	// (Straight right -> Counter-clokwise)
	/*
	** Vertex
	*/
	SVertexMainData vert_data;
	float vert_stride = XM_2PI / HorizontalDetail; // Side circle
	float horz_stride = XM_2PI / VerticalDetail; // Top-down circle
	float start = ((static_cast<float>(HorizontalDetail) / 4.0f) - 1.0f);
	for (uint8_t i = 0; i < VerticalDetail; ++i)
	{
		// Up center (= sphere center)
		vert_data.vVertices.push_back(SVertexMain(
			0,
			Radius,
			0,
			0, 0,
			Color.x, Color.y, Color.z, 1));

		// Up left down
		vert_data.vVertices.push_back(SVertexMain(
			Radius * cosf(horz_stride * i) * cosf(vert_stride * start),
			Radius * sinf(vert_stride * start),
			Radius * sinf(horz_stride * i) * cosf(vert_stride * start),
			0, 0,
			Color.x, Color.y, Color.z, 1));

		// Up right down
		vert_data.vVertices.push_back(SVertexMain(
			Radius * cosf(horz_stride * (i + 1)) * cosf(vert_stride * start),
			Radius * sinf(vert_stride * start),
			Radius * sinf(horz_stride * (i + 1)) * cosf(vert_stride * start),
			0, 0,
			Color.x, Color.y, Color.z, 1));

		// Down center (= sphere center)
		vert_data.vVertices.push_back(SVertexMain(
			0,
			-Radius,
			0,
			0, 0,
			Color.x, Color.y, Color.z, 1));

		// Down right down
		vert_data.vVertices.push_back(SVertexMain(
			Radius * cosf(horz_stride * (i + 1)) * cosf(vert_stride * start),
			-Radius * sinf(vert_stride * start),
			Radius * sinf(horz_stride * (i + 1)) * cosf(vert_stride * start),
			0, 0,
			Color.x, Color.y, Color.z, 1));

		// Down left down
		vert_data.vVertices.push_back(SVertexMain(
			Radius * cosf(horz_stride * i) * cosf(vert_stride * start),
			-Radius * sinf(vert_stride * start),
			Radius * sinf(horz_stride * i) * cosf(vert_stride * start),
			0, 0,
			Color.x, Color.y, Color.z, 1));
	}

	// if (HorizontalDetail == 4), which is the minimum size,
	// no sides are needed!
	if (HorizontalDetail > 4)
	{
		uint8_t real_horz_count = (HorizontalDetail / 2) - 1;

		for (uint8_t horz = 1; horz < real_horz_count; ++horz)
		{
			for (uint8_t vert = 0; vert < VerticalDetail; ++vert)
			{
				// Side #1 left up
				vert_data.vVertices.push_back(SVertexMain(
					Radius * cosf(horz_stride * (vert)) * cosf(vert_stride * (start + 1 - horz)),
					Radius * sinf(vert_stride * (start + 1 - horz)),
					Radius * sinf(horz_stride * (vert)) * cosf(vert_stride * (start + 1 - horz)),
					0, 0,
					Color.x, Color.y, Color.z, 1));

				// Side #1 left down
				vert_data.vVertices.push_back(SVertexMain(
					Radius * cosf(horz_stride * (vert)) * cosf(vert_stride * (start - horz)),
					Radius * sinf(vert_stride * (start - horz)),
					Radius * sinf(horz_stride * (vert)) * cosf(vert_stride * (start - horz)),
					0, 0,
					Color.x, Color.y, Color.z, 1));

				// Side #1 right up
				vert_data.vVertices.push_back(SVertexMain(
					Radius * cosf(horz_stride * (vert + 1)) * cosf(vert_stride * (start + 1 - horz)),
					Radius * sinf(vert_stride * (start + 1 - horz)),
					Radius * sinf(horz_stride * (vert + 1)) * cosf(vert_stride * (start + 1 - horz)),
					0, 0,
					Color.x, Color.y, Color.z, 1));

				// Side #2 right up
				vert_data.vVertices.push_back(SVertexMain(
					Radius * cosf(horz_stride * (vert + 1)) * cosf(vert_stride * (start + 1 - horz)),
					Radius * sinf(vert_stride * (start + 1 - horz)),
					Radius * sinf(horz_stride * (vert + 1)) * cosf(vert_stride * (start + 1 - horz)),
					0, 0,
					Color.x, Color.y, Color.z, 1));

				// Side #2 left down
				vert_data.vVertices.push_back(SVertexMain(
					Radius * cosf(horz_stride * (vert)) * cosf(vert_stride * (start - horz)),
					Radius * sinf(vert_stride * (start - horz)),
					Radius * sinf(horz_stride * (vert)) * cosf(vert_stride * (start - horz)),
					0, 0,
					Color.x, Color.y, Color.z, 1));

				// Side #2 right down
				vert_data.vVertices.push_back(SVertexMain(
					Radius * cosf(horz_stride * (vert + 1)) * cosf(vert_stride * (start - horz)),
					Radius * sinf(vert_stride * (start - horz)),
					Radius * sinf(horz_stride * (vert + 1)) * cosf(vert_stride * (start - horz)),
					0, 0,
					Color.x, Color.y, Color.z, 1));
			}
		}
	}

	/*
	** Index
	*/
	SIndex3Data ind_data;
	for (unsigned int i = 0; i < vert_data.GetCount() / 3; ++i)
	{
		// Clock-wise winding
		ind_data.vIndices.push_back(SIndex3(i * 3, i * 3 + 2, i * 3 + 1));
	}

	// Save data
	m_Mesh.VertexData = vert_data;
	m_Mesh.IndexData = ind_data;

	// Create buffers
	CreateBuffers();

	return *this;
}

void CMeshContainer::CreateBuffers() noexcept
{
	// Create vertex buffer
	D3D11_BUFFER_DESC vertex_buffer_description{};
	vertex_buffer_description.Usage = D3D11_USAGE_DEFAULT;
	vertex_buffer_description.ByteWidth = m_Mesh.VertexData.GetByteSize();
	vertex_buffer_description.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertex_buffer_description.CPUAccessFlags = 0;
	vertex_buffer_description.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertex_buffer_data{};
	vertex_buffer_data.pSysMem = m_Mesh.VertexData.GetPtrData();

	m_pDevice->CreateBuffer(&vertex_buffer_description, &vertex_buffer_data, &m_Mesh.VertexBuffer);

	// Create index buffer
	D3D11_BUFFER_DESC index_buffer_description{};
	index_buffer_description.Usage = D3D11_USAGE_DEFAULT;
	index_buffer_description.ByteWidth = m_Mesh.IndexData.GetByteSize();
	index_buffer_description.BindFlags = D3D11_BIND_INDEX_BUFFER;
	index_buffer_description.CPUAccessFlags = 0;
	index_buffer_description.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA index_buffer_data{};
	index_buffer_data.pSysMem = m_Mesh.IndexData.GetPtrData();

	m_pDevice->CreateBuffer(&index_buffer_description, &index_buffer_data, &m_Mesh.IndexBuffer);
}

auto CMeshContainer::SetMatrix(XMMATRIX World, XMMATRIX ViewProjection) noexcept->CMeshContainer&
{
	XMMATRIX world = World;
	m_CBMatrixData.WVP = world * ViewProjection;

	return *this;
}

void CMeshContainer::DrawMesh() noexcept
{
	// Set IA primitive topology
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Set IA vertex buffer
	m_pDeviceContext->IASetVertexBuffers(
		0, 1, &m_Mesh.VertexBuffer, m_Mesh.VertexData.GetPtrStride(), m_Mesh.VertexData.GetPtrOffset());

	// Set IA index buffer
	m_pDeviceContext->IASetIndexBuffer(m_Mesh.IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Draw indexed
	m_pDeviceContext->DrawIndexed(m_Mesh.IndexData.GetCount(), 0, 0);
}