#include "CDX11Minimal.h"
#include "CInput.h"
#include "CCamera.h"
#include "CMeshContainer.h"

void OnInput();
void OnRender();

static constexpr auto	KWindowPosition{ SPosition(0, 30) };
static constexpr auto	KWindowSize{ SSize(800, 600) };

static CDX11Minimal		myDX;
static CInput			myInput;
static CCamera			myCamera;
static CMeshContainer	myMeshMaker;

int main()
{
	myDX.Create(KWindowPosition, KWindowSize, "HLSL Tester")
		.AddVS(L"HLSL/VSMain.hlsl", KInputElementDescriptionMain, ARRAYSIZE(KInputElementDescriptionMain))
		.AddPS(L"HLSL/PSMain.hlsl")
		.AddCB(sizeof(SCBMatrixData))
		.SetVS(0)
		.SetPS(0)
		.SetFunctionOnInput(OnInput)
		.SetFunctionOnRender(OnRender);

	myInput.Create(myDX.GetHINSTANCE(), myDX.GetHWND());

	myCamera.Create(KWindowSize)
		.SetCameraType(ECameraType::FreeLook)
		.SetPosition(XMFLOAT3(0, 0, -5.0f))
		.SetLookAt(XMFLOAT3(0, 0, 0));

	myMeshMaker
		.Create(myDX.GetPtrDevice(), myDX.GetPtrDeviceContext())
		.MakeSphere(1.0f, 16, 7, XMFLOAT3(0.0f, 0.6f, 0.0f));

	myDX.Run();
	
	return 0;
}

void OnInput()
{
	auto& state = myInput.GetDeviceState();

	if (state.Keys[DIK_ESCAPE])
	{
		myDX.Terminate();
	}

	if (state.Keys[DIK_W])
	{
		myCamera.MoveCamera(ECameraMoveDirection::Forward, 1.0f);
	}

	if (state.Keys[DIK_S])
	{
		myCamera.MoveCamera(ECameraMoveDirection::Backward, 1.0f);
	}

	if (state.Keys[DIK_A])
	{
		myCamera.MoveCamera(ECameraMoveDirection::Left, 1.0f);
	}

	if (state.Keys[DIK_D])
	{
		myCamera.MoveCamera(ECameraMoveDirection::Right, 1.0f);
	}

	if ((state.CurrentMouse.lX != state.PreviousMouse.lX) || (state.CurrentMouse.lY != state.PreviousMouse.lY))
	{
		if (state.CurrentMouse.rgbButtons[1])
		{
			myCamera.RotateCamera(static_cast<float>(state.CurrentMouse.lY), static_cast<float>(state.CurrentMouse.lX), 0);
		}
	}
}

void OnRender()
{
	myMeshMaker.SetMatrix(XMMatrixIdentity(), myCamera.GetViewProjectionMatrix());

	myDX.UpdateCB(0, EShaderType::VS, 0, myMeshMaker.GetCBMatrixData());

	myMeshMaker.DrawMesh();
}