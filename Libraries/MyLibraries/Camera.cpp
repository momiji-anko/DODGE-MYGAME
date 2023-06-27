//
// Camera.cpp
//
#include "pch.h"
#include "Camera.h"
#include<vector>
#include "DeviceResources.h"



// �萔�̏�����
const DirectX::SimpleMath::Vector3 Camera::CAMERA_DISTANCE_POSITION = DirectX::SimpleMath::Vector3(0.0f, 18.0f, 18.0f);
const DirectX::SimpleMath::Vector3 Camera::CAMERA_TARGET_POSITION = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 3.0f);

//-------------------------------------------------------------------
// �R���X�g���N�^
//-------------------------------------------------------------------
Camera::Camera()
	: m_view(DirectX::SimpleMath::Matrix::Identity)
	, m_eye(CAMERA_DISTANCE_POSITION)
	, m_target(CAMERA_TARGET_POSITION)
	, m_up(DirectX::SimpleMath::Vector3::Up)
	, m_shake(0)
{

	CalculateViewMatrix();
	CalculateProjectionMatrix();
}


//-------------------------------------------------------------------
// �f�X�g���N�^
//-------------------------------------------------------------------
Camera::~Camera()
{
}



//-------------------------------------------------------------------
// �r���[�s��̎Z�o
//-------------------------------------------------------------------
void Camera::CalculateViewMatrix()
{
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_eye, m_target, m_up);
}

//-------------------------------------------------------------------
// �v���W�F�N�V�����s��̎Z�o
//-------------------------------------------------------------------
void Camera::CalculateProjectionMatrix()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	// �E�B���h�E�T�C�Y�̎擾
	float width = static_cast<float>(pDR->GetOutputSize().right);
	float height = static_cast<float>(pDR->GetOutputSize().bottom);

	constexpr float fieldOfView = DirectX::XMConvertToRadians(45.0f);    // ��p
	float aspectRatio = width / height;							// ��ʏc����
	float nearPlane = 1.0f;                                     // �J���������ԋ߂����e��
	float farPlane = 1000.0f;                                    // �J���������ԉ������e��
	m_projection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(fieldOfView, aspectRatio, nearPlane, farPlane);
}

void Camera::ShakeCamera()
{
	DirectX::SimpleMath::Vector3 target = { CAMERA_TARGET_POSITION };

	//�h����
	std::vector<DirectX::SimpleMath::Vector3> shakePosition=
	{
		DirectX::SimpleMath::Vector3(0.0f,3.0f,0.0f),
		DirectX::SimpleMath::Vector3(0.0f,3.0f,0.0f),
		DirectX::SimpleMath::Vector3(0.0f,-3.0f,0.0f),
		DirectX::SimpleMath::Vector3(0.0f,-3.0f,0.0f),
		DirectX::SimpleMath::Vector3(0.0f,3.0f,0.0f),
		DirectX::SimpleMath::Vector3(0.0f,3.0f,0.0f),
		DirectX::SimpleMath::Vector3::Zero
	};
	
	//�^�[�Q�b�g�̈ʒu��ς��ăJ������h�炷
	m_target = shakePosition[m_shake] + target;

	
	m_shake++;

	int max = shakePosition.size()-1;

	m_shake = std::min(m_shake, max);

	//�r���[�s��̌v�Z
	CalculateViewMatrix();

}