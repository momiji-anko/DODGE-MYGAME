//
// Camera.cpp
//
#include "pch.h"
#include "Camera.h"
#include<vector>
#include "DeviceResources.h"



// 定数の初期化
const DirectX::SimpleMath::Vector3 Camera::CAMERA_DISTANCE_POSITION = DirectX::SimpleMath::Vector3(0.0f, 18.0f, 18.0f);
const DirectX::SimpleMath::Vector3 Camera::CAMERA_TARGET_POSITION = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 3.0f);

//-------------------------------------------------------------------
// コンストラクタ
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
// デストラクタ
//-------------------------------------------------------------------
Camera::~Camera()
{
}



//-------------------------------------------------------------------
// ビュー行列の算出
//-------------------------------------------------------------------
void Camera::CalculateViewMatrix()
{
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_eye, m_target, m_up);
}

//-------------------------------------------------------------------
// プロジェクション行列の算出
//-------------------------------------------------------------------
void Camera::CalculateProjectionMatrix()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	// ウィンドウサイズの取得
	float width = static_cast<float>(pDR->GetOutputSize().right);
	float height = static_cast<float>(pDR->GetOutputSize().bottom);

	constexpr float fieldOfView = DirectX::XMConvertToRadians(45.0f);    // 画角
	float aspectRatio = width / height;							// 画面縦横比
	float nearPlane = 1.0f;                                     // カメラから一番近い投影面
	float farPlane = 1000.0f;                                    // カメラから一番遠い投影面
	m_projection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(fieldOfView, aspectRatio, nearPlane, farPlane);
}

void Camera::ShakeCamera()
{
	DirectX::SimpleMath::Vector3 target = { CAMERA_TARGET_POSITION };

	//揺れる量
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
	
	//ターゲットの位置を変えてカメラを揺らす
	m_target = shakePosition[m_shake] + target;

	
	m_shake++;

	int max = shakePosition.size()-1;

	m_shake = std::min(m_shake, max);

	//ビュー行列の計算
	CalculateViewMatrix();

}