#include "CInput.h"

CInput::~CInput()
{
	if (m_pMouseDevice)
	{
		m_pMouseDevice->Unacquire();
		RELEASE_NULLPTR(m_pMouseDevice);
	}
	if (m_pKeyboardDevice)
	{
		m_pKeyboardDevice->Unacquire();
		RELEASE_NULLPTR(m_pKeyboardDevice);
	}

	RELEASE_NULLPTR(m_pDirectInput);
}

void CInput::Create(const HINSTANCE hInstance, const HWND hWnd)
{
	m_hWnd = hWnd;

	assert(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&m_pDirectInput, nullptr) == S_OK);

	//CreateMouseDevice(DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);
	CreateMouseDevice(DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	CreateKeyboardDevice(DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
}

void CInput::CreateMouseDevice(DWORD dwFlags)
{
	assert(m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pMouseDevice, nullptr) == S_OK);
	
	assert(m_pMouseDevice->SetDataFormat(&c_dfDIMouse) == S_OK);

	assert(m_pMouseDevice->SetCooperativeLevel(m_hWnd, dwFlags) == S_OK);

	assert(m_pMouseDevice->Acquire() == S_OK);
}

void CInput::CreateKeyboardDevice(DWORD dwFlags)
{
	assert(m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pKeyboardDevice, nullptr) == S_OK);

	assert(m_pKeyboardDevice->SetDataFormat(&c_dfDIKeyboard) == S_OK);

	assert(m_pKeyboardDevice->SetCooperativeLevel(m_hWnd, dwFlags) == S_OK);

	assert(m_pKeyboardDevice->Acquire() == S_OK);
}

void CInput::UpdateDeviceState() noexcept
{
	m_DeviceState.PreviousMouse = m_DeviceState.CurrentMouse;

	m_pMouseDevice->GetDeviceState(sizeof(m_DeviceState.CurrentMouse), (LPVOID)&m_DeviceState.CurrentMouse);
	m_pKeyboardDevice->GetDeviceState(sizeof(m_DeviceState.Keys), (LPVOID)&m_DeviceState.Keys);
}

auto CInput::GetDeviceState() noexcept->SDirectInputDeviceState&
{
	UpdateDeviceState();

	return m_DeviceState;
}