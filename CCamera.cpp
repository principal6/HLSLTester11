#include "CCamera.h"

auto CCamera::Create(SSize WindowSize) noexcept->CCamera&
{
	// Set default camera information
	m_CameraUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	m_CameraPosition = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	m_CameraLookAt = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	m_CameraDefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	m_CameraDefaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	
	// Get window size in float
	float width = static_cast<float>(WindowSize.X);
	float height = static_cast<float>(WindowSize.Y);
	
	// Get projection matrix
	m_MatrixProjection = XMMatrixPerspectiveFovLH(KFOV * XM_PI, width / height, KNearZ, KFarZ);

	return *this;
}

void CCamera::MoveCamera(ECameraMoveDirection Direction, float Stride) noexcept
{
	Stride = max(Stride, 0);

	XMVECTOR camera_forward = m_CameraForward;
	XMVECTOR camera_right = m_CameraRight;
	
	switch (m_CameraType)
	{
	case ECameraType::FirstPerson:
		camera_forward = GetFirstPersonForward();
		camera_right = GetFirstPersonRight();
		Stride *= KFactor;
		break;
	case ECameraType::ThirdPerson:
		UpdateCamera();
		Stride *= KFactor;
		return;
		//break;
	case ECameraType::FreeLook:
		Stride *= KFactor;
		break;
	default:
		break;
	}

	switch (Direction)
	{
	case ECameraMoveDirection::Left:
		m_CameraPosition -= Stride * camera_right;
		break;
	case ECameraMoveDirection::Right:
		m_CameraPosition += Stride * camera_right;
		break;
	case ECameraMoveDirection::Forward:
		m_CameraPosition += Stride * camera_forward;
		break;
	case ECameraMoveDirection::Backward:
		m_CameraPosition -= Stride * camera_forward;
		break;
	default:
		break;
	}

	UpdateCamera();
}

auto CCamera::GetFirstPersonForward() noexcept->XMVECTOR
{
	XMMATRIX camera_rotation_matrix = XMMatrixRotationRollPitchYaw(0, m_Yaw, 0);
	return XMVector3TransformCoord(m_CameraDefaultForward, camera_rotation_matrix);
}

auto CCamera::GetFirstPersonRight() noexcept->XMVECTOR
{
	XMMATRIX camera_rotation_matrix = XMMatrixRotationRollPitchYaw(0, m_Yaw, 0);
	return XMVector3TransformCoord(m_CameraDefaultRight, camera_rotation_matrix);
}

void CCamera::RotateCamera(float PitchStride, float YawStride, float RollStride) noexcept
{
	if (m_CameraType == ECameraType::ThirdPerson)
	{
		m_Pitch -= PitchStride * KFactor;
	}
	else
	{
		m_Pitch += PitchStride * KFactor;
	}
	
	m_Yaw += YawStride * KFactor;
	m_Roll += RollStride * KFactor;

	m_Pitch = min(m_Pitch, +XM_PIDIV2 - 0.01f);
	m_Pitch = max(m_Pitch, -XM_PIDIV2 + 0.01f);

	UpdateCamera();
}

void CCamera::ZoomCamera(float Factor) noexcept
{
	if (m_CameraType == ECameraType::ThirdPerson)
	{
		Factor *= -KFactor * 0.1f;

		XMVECTOR LookDistance = XMVector3Length(m_CameraPosition - m_CameraLookAt);
		LookDistance *= (1.0f + Factor);
		m_CameraPosition = XMVector3Normalize(m_CameraForward) * LookDistance;
	}
}

void CCamera::UpdateCamera() noexcept
{
	switch (m_CameraType)
	{
	case ECameraType::FirstPerson:
		UpdateFirstPersonOrFreeLookCamera();
		break;
	case ECameraType::ThirdPerson:
		UpdateThirdPersonCamera();
		break;
	case ECameraType::FreeLook:
		UpdateFirstPersonOrFreeLookCamera();
		break;
	default:
		break;
	}
}

void CCamera::UpdateFirstPersonOrFreeLookCamera() noexcept
{
	XMMATRIX camera_rotation_matrix = XMMatrixRotationRollPitchYaw(m_Pitch, m_Yaw, m_Roll);

	m_CameraForward = XMVector3TransformCoord(m_CameraDefaultForward, camera_rotation_matrix);
	m_CameraRight = XMVector3TransformCoord(m_CameraDefaultRight, camera_rotation_matrix);

	m_CameraLookAt = XMVector3Normalize(m_CameraForward);
	m_CameraLookAt = m_CameraPosition + m_CameraLookAt;
}

void CCamera::UpdateThirdPersonCamera() noexcept
{
	XMMATRIX camera_rotation_matrix = XMMatrixRotationRollPitchYaw(m_Pitch, m_Yaw, m_Roll);

	m_CameraForward = XMVector3TransformCoord(m_CameraDefaultForward, camera_rotation_matrix);
	m_CameraRight = XMVector3TransformCoord(m_CameraDefaultRight, camera_rotation_matrix);

	XMVECTOR LookDistance = XMVector3Length(m_CameraPosition - m_CameraLookAt);
	m_CameraPosition = XMVector3Normalize(m_CameraForward) * LookDistance;
}

auto CCamera::SetCameraType(ECameraType Type) noexcept->CCamera&
{
	m_CameraType = Type;
	return *this;
}

auto CCamera::SetPosition(XMFLOAT3 Position) noexcept->CCamera&
{
	m_CameraPosition = XMVectorSet(Position.x, Position.y, Position.z, 0);
	return *this;
}

auto CCamera::SetLookAt(XMFLOAT3 LookAt) noexcept->CCamera&
{
	m_CameraLookAt = XMVectorSet(LookAt.x, LookAt.y, LookAt.z, 0);
	return *this;
}

auto CCamera::GetPosition() const noexcept->XMVECTOR
{
	return m_CameraPosition;
}

auto CCamera::GetPositionFloat4() const noexcept->XMFLOAT4
{
	XMFLOAT4 result{};
	XMStoreFloat4(&result, m_CameraPosition);
	return result;
}

auto CCamera::GetLookAt() const noexcept->XMVECTOR
{
	return m_CameraLookAt;
}

auto CCamera::GetForward() const noexcept->XMVECTOR
{
	return m_CameraForward;
}

auto CCamera::GetRight() const noexcept->XMVECTOR
{
	return m_CameraRight;
}

auto CCamera::GetViewMatrix() const noexcept->XMMATRIX
{
	// Update view matrix
	m_MatrixView = XMMatrixLookAtLH(m_CameraPosition, m_CameraLookAt, m_CameraUp);

	return m_MatrixView;
}

auto CCamera::GetProjectionMatrix() const noexcept->XMMATRIX
{
	return m_MatrixProjection;
}

auto CCamera::GetViewProjectionMatrix() const noexcept->XMMATRIX
{
	return GetViewMatrix() * m_MatrixProjection;
}