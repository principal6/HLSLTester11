#pragma once

#include "SharedHeader.h"

using FPOnRender = void(*)();
using FPOnInput = void(*)();

enum class EShaderType
{
	VS,
	PS,
};

struct SVS
{
	ID3DBlob*				Blob{};
	ID3D11VertexShader*		Shader{};
	ID3D11InputLayout*		InputLayout{};

	~SVS()
	{
		RELEASE_NULLPTR(Blob);
		RELEASE_NULLPTR(Shader);
		RELEASE_NULLPTR(InputLayout);
	}
};

struct SPS
{
	ID3DBlob*				Blob{};
	ID3D11PixelShader*		Shader{};

	~SPS()
	{
		RELEASE_NULLPTR(Blob);
		RELEASE_NULLPTR(Shader);
	}
};

class CDX11Minimal
{
public:
	~CDX11Minimal();

	auto Create(SPosition Position, SSize Size, const char* Title) noexcept->CDX11Minimal&;

	auto AddVS(wstring FileName, const D3D11_INPUT_ELEMENT_DESC* InputLayoutDesc, uint32_t InputElementCount) noexcept->CDX11Minimal&;
	auto AddPS(wstring FileName) noexcept->CDX11Minimal&;
	auto AddCB(uint32_t SizeOfStructure) noexcept->CDX11Minimal&;

	auto SetVS(uint32_t Index) noexcept->CDX11Minimal&;
	auto SetPS(uint32_t Index) noexcept->CDX11Minimal&;
	auto SetMainRenderTargetDepthStencil() noexcept->CDX11Minimal&;
	auto SetMainViewport() noexcept->CDX11Minimal&;

	auto SetFunctionOnRender(FPOnRender Function) noexcept->CDX11Minimal&;
	auto SetFunctionOnInput(FPOnInput Function) noexcept->CDX11Minimal&;

	auto UpdateCB(uint32_t Index, EShaderType Shader, uint32_t Slot, const void* Data) noexcept->CDX11Minimal&;

	void Run() noexcept;
	void Terminate() noexcept;

	auto GetHWND() const noexcept { return m_hWnd; }
	auto GetHINSTANCE() const noexcept { return m_hInstance; }
	auto GetPtrDevice() const noexcept { return m_Device; }
	auto GetPtrDeviceContext() const noexcept { return m_DeviceContext; }

private:
	void CreateWindowsWindow(const char* Title) noexcept;

	void CreateDeviceAndSwapChain() noexcept;
	void CreateMainRenderTargetView() noexcept;
	void CreateDepthStencilView() noexcept;
	void CreateMainViewport() noexcept;

private:
	HINSTANCE				m_hInstance{};
	HWND					m_hWnd{};
	SPosition				m_Position{};
	SSize					m_Size{};

	IDXGISwapChain*			m_SwapChain{};
	ID3D11Device*			m_Device{};
	ID3D11DeviceContext*	m_DeviceContext{};

	ID3D11Texture2D*		m_BackBuffer{};
	ID3D11RenderTargetView*	m_BackBufferRTV{};

	ID3D11Texture2D*		m_DepthStencilBuffer{};
	ID3D11DepthStencilView* m_DepthStencilView{};

	D3D11_VIEWPORT			m_MainViewport{};

	vector<SVS>				m_vVS{};
	vector<SPS>				m_vPS{};
	vector<ID3D11Buffer*>	m_vCB{};

	FPOnRender				m_fpOnRender{};
	FPOnInput				m_fpOnInput{};
};