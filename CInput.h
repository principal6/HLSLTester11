#pragma once

#define DIRECTINPUT_VERSION 0x0800

#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

#include "SharedHeader.h"
#include <dinput.h>

static constexpr uint16_t KInputKeyCount{ 256 };

struct SDirectInputDeviceState
{
	char Keys[KInputKeyCount]{};
	DIMOUSESTATE CurrentMouse{};
	DIMOUSESTATE PreviousMouse{};
};

class CInput final
{
public:
	CInput() = default;
	~CInput();

	void Create(const HINSTANCE hInstance, const HWND hWnd);
	auto GetDeviceState() noexcept->SDirectInputDeviceState&;

private:
	void CreateMouseDevice(DWORD dwFlags);
	void CreateKeyboardDevice(DWORD dwFlags);

	void UpdateDeviceState() noexcept;

private:
	HWND m_hWnd{};

	LPDIRECTINPUT8			m_pDirectInput{};
	LPDIRECTINPUTDEVICE8	m_pKeyboardDevice{};
	LPDIRECTINPUTDEVICE8	m_pMouseDevice{};

	SDirectInputDeviceState	m_DeviceState{};
};