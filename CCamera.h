#pragma once

#include "SharedHeader.h"

enum class ECameraType
{
	Invalid,

	FirstPerson,
	ThirdPerson,
	FreeLook,
};
	
enum class ECameraMoveDirection
{
	Left,
	Right,
	Forward,
	Backward,
};

class CCamera
{
public:
	CCamera() = default;
	~CCamera() = default;

	// Called in JWGame class
	auto Create(SSize WindowSize) noexcept->CCamera&;

	void MoveCamera(ECameraMoveDirection Direction, float Stride = 1.0f) noexcept;
	void RotateCamera(float PitchStride, float YawStride, float RollStride) noexcept;

	// Only used for third-person camera
	void ZoomCamera(float Factor) noexcept;

	auto SetCameraType(ECameraType Type) noexcept->CCamera&;
	auto SetPosition(XMFLOAT3 Position) noexcept->CCamera&;
	auto SetLookAt(XMFLOAT3 LookAt) noexcept->CCamera&;

	auto GetPosition() const noexcept->XMVECTOR;
	auto GetPositionFloat4() const noexcept->XMFLOAT4;
	auto GetLookAt() const noexcept->XMVECTOR;
	auto GetForward() const noexcept->XMVECTOR;
	auto GetRight() const noexcept->XMVECTOR;

	auto GetViewMatrix() const noexcept->XMMATRIX;
	auto GetProjectionMatrix() const noexcept->XMMATRIX;
	auto GetViewProjectionMatrix() const noexcept->XMMATRIX;

private:
	auto GetFirstPersonForward() noexcept->XMVECTOR;
	auto GetFirstPersonRight() noexcept->XMVECTOR;

	void UpdateCamera() noexcept;

	void UpdateFirstPersonOrFreeLookCamera() noexcept;
	void UpdateThirdPersonCamera() noexcept;

private:
	static constexpr float KFOV{ 0.25f };
	static constexpr float KNearZ{ 0.1f };
	static constexpr float KFarZ{ 1000.0f };
	static constexpr float KFactor{ 0.01f };
	
	mutable XMMATRIX m_MatrixView{};
	mutable XMMATRIX m_MatrixProjection{};

	ECameraType m_CameraType{ ECameraType::Invalid };

	XMVECTOR m_CameraUp{};
	XMVECTOR m_CameraPosition{};
	XMVECTOR m_CameraLookAt{};

	XMVECTOR m_CameraDefaultForward{};
	XMVECTOR m_CameraDefaultRight{};
	XMVECTOR m_CameraForward{};
	XMVECTOR m_CameraRight{};

	// Y-Z rotation (nod)
	float m_Pitch{};
		
	// X-Z rotation (turn)
	float m_Yaw{};

	// Y-X rotation (tilt)
	float m_Roll{};

	XMMATRIX m_CameraRotationMatrix{};
};