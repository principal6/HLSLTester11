#include "CDX11Minimal.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CHAR:
		break;
	case WM_KEYDOWN:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

CDX11Minimal::~CDX11Minimal()
{
	RELEASE_NULLPTR(m_SwapChain);
	RELEASE_NULLPTR(m_Device);
	RELEASE_NULLPTR(m_DeviceContext);

	m_vVS.clear();
	m_vPS.clear();
	for (auto& iter : m_vCB)
	{
		RELEASE_NULLPTR(iter);
	}
	
	RELEASE_NULLPTR(m_BackBuffer);
	RELEASE_NULLPTR(m_BackBufferRTV);

	RELEASE_NULLPTR(m_DepthStencilBuffer);
	RELEASE_NULLPTR(m_DepthStencilView);
}

auto CDX11Minimal::Create(SPosition Position, SSize Size, const char* Title) noexcept->CDX11Minimal&
{
	m_Position = Position;
	m_Size = Size;
	
	CreateWindowsWindow(Title);

	CreateDeviceAndSwapChain();
	
	CreateMainRenderTargetView();
	CreateDepthStencilView();
	CreateMainViewport();

	SetMainRenderTargetDepthStencil();
	SetMainViewport();

	return *this;
}

void CDX11Minimal::CreateWindowsWindow(const char* Title) noexcept
{
	m_hInstance = GetModuleHandleA(nullptr);

	WNDCLASSEXA wc{};
	wc.cbSize = sizeof(WNDCLASSEXA);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIconA(nullptr, IDI_APPLICATION);
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = "MinimalDX";
	wc.hIconSm = LoadIconA(nullptr, IDI_APPLICATION);

	assert(RegisterClassExA(&wc));

	RECT rect{};
	rect.left = m_Position.X;
	rect.top = m_Position.Y;
	rect.right = m_Position.X + m_Size.X;
	rect.bottom = m_Position.Y + m_Size.Y;

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	assert(m_hWnd = CreateWindowExA(0, wc.lpszClassName, Title,
		WS_OVERLAPPEDWINDOW,
		rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
		nullptr, nullptr, m_hInstance, nullptr));

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);
}

void CDX11Minimal::CreateDeviceAndSwapChain() noexcept
{
	// ȭ�� ���� ����
	DXGI_MODE_DESC buffer_description{};
	buffer_description.Width = m_Size.X;
	buffer_description.Height = m_Size.Y;
	buffer_description.RefreshRate.Numerator = 60;
	buffer_description.RefreshRate.Denominator = 1;
	buffer_description.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	buffer_description.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	buffer_description.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// ����ü�� ����
	DXGI_SWAP_CHAIN_DESC swap_chain_description{};
	swap_chain_description.BufferDesc = buffer_description;
	swap_chain_description.SampleDesc.Count = 1;
	swap_chain_description.SampleDesc.Quality = 0;
	swap_chain_description.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_description.BufferCount = 1;
	swap_chain_description.OutputWindow = m_hWnd;
	swap_chain_description.Windowed = TRUE;
	swap_chain_description.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// DirectX ��ġ(Device)�� ��ġ ����(DeviceContext), ����ü��(SwapChain)�� ���ÿ� �����.
	D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
		nullptr, 0, D3D11_SDK_VERSION, &swap_chain_description, &m_SwapChain,
		&m_Device, nullptr, &m_DeviceContext);
}

void CDX11Minimal::CreateMainRenderTargetView() noexcept
{
	// ���� Ÿ������ ���� ����� ������
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&m_BackBuffer);

	// ����� ���� Ÿ�� �� ����
	m_Device->CreateRenderTargetView(m_BackBuffer, nullptr, &m_BackBufferRTV);
}

void CDX11Minimal::CreateDepthStencilView() noexcept
{
	// ���� ���ٽ� �ؽ�ó ����
	D3D11_TEXTURE2D_DESC depth_stencil_desc{};
	depth_stencil_desc.Width = m_Size.X;
	depth_stencil_desc.Height = m_Size.Y;
	depth_stencil_desc.MipLevels = 1;
	depth_stencil_desc.ArraySize = 1;
	depth_stencil_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depth_stencil_desc.SampleDesc.Count = 1;
	depth_stencil_desc.SampleDesc.Quality = 0;
	depth_stencil_desc.Usage = D3D11_USAGE_DEFAULT;
	depth_stencil_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depth_stencil_desc.CPUAccessFlags = 0;
	depth_stencil_desc.MiscFlags = 0;

	// ���� ���ٽ� �ؽ�ó ����
	m_Device->CreateTexture2D(&depth_stencil_desc, nullptr, &m_DepthStencilBuffer);

	// ���� ���ٽ� �� ����
	m_Device->CreateDepthStencilView(m_DepthStencilBuffer, nullptr, &m_DepthStencilView);
}

void CDX11Minimal::CreateMainViewport() noexcept
{
	// ����Ʈ ����
	m_MainViewport.TopLeftX = 0.0f;
	m_MainViewport.TopLeftY = 0.0f;
	m_MainViewport.Width = static_cast<float>(m_Size.X);
	m_MainViewport.Height = static_cast<float>(m_Size.Y);
	m_MainViewport.MinDepth = 0.0f;
	m_MainViewport.MaxDepth = 1.0f;
}

auto CDX11Minimal::AddVS(wstring FileName, const D3D11_INPUT_ELEMENT_DESC* InputLayoutDesc, uint32_t InputElementCount) noexcept->CDX11Minimal&
{
	m_vVS.push_back(SVS());
	auto& vs = m_vVS[m_vVS.size() - 1];

	// ���̴��� ���Ϸκ��� �������Ѵ�.
	D3DCompileFromFile(FileName.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_4_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vs.Blob, nullptr);

	// ���� ���̴�(Vertex Shader) ����
	m_Device->CreateVertexShader(vs.Blob->GetBufferPointer(), vs.Blob->GetBufferSize(), nullptr, &vs.Shader);

	// ��ǲ ���̾ƿ� ����
	m_Device->CreateInputLayout(InputLayoutDesc, InputElementCount, vs.Blob->GetBufferPointer(), vs.Blob->GetBufferSize(), &vs.InputLayout);

	return *this;
}

auto CDX11Minimal::AddPS(wstring FileName) noexcept->CDX11Minimal&
{
	m_vPS.push_back(SPS());
	auto& ps = m_vPS[m_vPS.size() - 1];

	// ���̴��� ���Ϸκ��� �������Ѵ�.
	D3DCompileFromFile(FileName.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_4_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &ps.Blob, nullptr);

	// �ȼ� ���̴�(Pixel Shader) ����
	m_Device->CreatePixelShader(ps.Blob->GetBufferPointer(), ps.Blob->GetBufferSize(), nullptr, &ps.Shader);

	return *this;
}

auto CDX11Minimal::AddCB(uint32_t SizeOfStructure) noexcept->CDX11Minimal&
{
	m_vCB.push_back(nullptr);
	auto& cb = m_vCB[m_vCB.size() - 1];

	// ��� ���� ����
	D3D11_BUFFER_DESC constant_buffer_des{};
	constant_buffer_des.Usage = D3D11_USAGE_DEFAULT;
	constant_buffer_des.ByteWidth = SizeOfStructure;
	constant_buffer_des.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constant_buffer_des.CPUAccessFlags = 0;
	constant_buffer_des.MiscFlags = 0;

	// ��� ���� ����
	m_Device->CreateBuffer(&constant_buffer_des, nullptr, &cb);

	return *this;
}

auto CDX11Minimal::SetVS(uint32_t Index) noexcept->CDX11Minimal&
{
	assert(m_vVS.size());

	Index = min(Index, m_vVS.size() - 1);

	// VS ���̴� ����
	m_DeviceContext->VSSetShader(m_vVS[Index].Shader, nullptr, 0);

	// IA ��ǲ ���̾ƿ� ����
	m_DeviceContext->IASetInputLayout(m_vVS[Index].InputLayout);
	
	return *this;
}

auto CDX11Minimal::SetPS(uint32_t Index) noexcept->CDX11Minimal&
{
	assert(m_vPS.size());

	Index = min(Index, m_vPS.size() - 1);

	// PS ���̴� ����
	m_DeviceContext->PSSetShader(m_vPS[Index].Shader, nullptr, 0);

	return *this;
}

auto CDX11Minimal::SetMainRenderTargetDepthStencil() noexcept->CDX11Minimal&
{
	// OM ���� Ÿ�� �� & ���� ���ٽ� �� ����
	m_DeviceContext->OMSetRenderTargets(1, &m_BackBufferRTV, m_DepthStencilView);

	return *this;
}

auto CDX11Minimal::SetMainViewport() noexcept->CDX11Minimal&
{
	// RS ����Ʈ ����
	m_DeviceContext->RSSetViewports(1, &m_MainViewport);

	return *this;
}

auto CDX11Minimal::SetFunctionOnRender(FPOnRender Function) noexcept->CDX11Minimal&
{
	m_fpOnRender = Function;

	return *this;
}

auto CDX11Minimal::SetFunctionOnInput(FPOnInput Function) noexcept->CDX11Minimal&
{
	m_fpOnInput = Function;

	return *this;
}

auto CDX11Minimal::UpdateCB(uint32_t Index, EShaderType Shader, uint32_t Slot, const void* Data) noexcept->CDX11Minimal&
{
	assert(m_vCB.size());

	Index = min(Index, m_vCB.size() - 1);

	// ��� ���� ������Ʈ
	m_DeviceContext->UpdateSubresource(m_vCB[Index], 0, nullptr, Data, 0, 0);

	switch (Shader)
	{
	case EShaderType::VS:
		// VS ��� ���� ����
		m_DeviceContext->VSSetConstantBuffers(Slot, 1, &m_vCB[Index]);
		break;
	case EShaderType::PS:
		// PS ��� ���� ����
		m_DeviceContext->PSSetConstantBuffers(Slot, 1, &m_vCB[Index]);
		break;
	default:
		break;
	}

	return *this;
}

void CDX11Minimal::Run() noexcept
{
	MSG msg{};

	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// �Է� ó�� �Լ� ȣ��
			m_fpOnInput();

			// ���� Ÿ�� ���۸� �ʱ�ȭ�� ���� ����
			FLOAT clear_color[]{ 0.0f, 0.0f, 0.2f, 1.0f };

			// ���� Ÿ�� ���� ����
			m_DeviceContext->ClearRenderTargetView(m_BackBufferRTV, clear_color);

			// ���� ���ٽ� ���� ����
			m_DeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

			// ������ ó�� �Լ� ȣ��
			m_fpOnRender();

			// ���� Ÿ�� ���۸� ȭ�鿡 ǥ���ϱ�(Present)
			m_SwapChain->Present(0, 0);
		}
	}
}

void CDX11Minimal::Terminate() noexcept
{
	PostQuitMessage(0);
}