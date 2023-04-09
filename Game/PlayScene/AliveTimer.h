#pragma once

#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <CommonStates.h>
#include"../../Libraries/MyLibraries/Camera.h"
#include"StepTimer.h"
#include"Libraries/MyLibraries/Singleton.h"
#include<vector>

class AliveTimer : public Singleton< AliveTimer>
{

public:
	enum class TimeType
	{
		COUNTDOWN,
		NORMAL,
	};
private:
	float m_timer_s;

	// �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_numTexture;

	DirectX::CommonStates* m_commonState;

public:
	AliveTimer();
	~AliveTimer();

	// ������
	void Initialize(DirectX::CommonStates* commonState);

	// �X�V
	void Update(const DX::StepTimer& timer);

	// �`��
	void Draw();

	// �I������
	void Finalize();

	float GetTimer() { return m_timer_s; }


};