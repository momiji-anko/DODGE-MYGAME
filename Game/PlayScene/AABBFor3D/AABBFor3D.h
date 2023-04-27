/*
	AABBFor3D.h
	3D空間でAABBの当たり判定を置こうなうために
	機能をプログラムするクラスのヘッダのファイル


*/
#pragma once

#include<CommonStates.h>
#include<PrimitiveBatch.h>
#include<SimpleMath.h>
#include<VertexTypes.h>
#include<Effects.h>


//AABBの当たり判定
class AABBFor3D
{
public:
	//AABB用の当たり判定データを設定する
	void SetData(DirectX::SimpleMath::Vector3 minPos, DirectX::SimpleMath::Vector3 maxPos);

public:
	//コンストラクタ
	AABBFor3D();
	//デストラクタ
	~AABBFor3D();

	//初期化
	void Initialize();
	//描画
	void Draw(DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection, DirectX::XMFLOAT4 color);

	//当たり判定を行う
	bool HitCheck(AABBFor3D* other);

private:

	void DrawBox(DirectX::XMFLOAT4 color);

private:
	//中心座標
	DirectX::SimpleMath::Vector3 m_center;
	//各辺までの距離
	DirectX::SimpleMath::Vector3 m_r;

	//コモンステート
	std::unique_ptr<DirectX::CommonStates> m_commonState;
	//プリミティブバッチ
	std::unique_ptr < DirectX::PrimitiveBatch < DirectX::VertexPositionColor>> m_primitiveBach;

	//ベーシックエフェクト
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	//インプットレイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

};