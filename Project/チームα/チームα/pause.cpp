//=============================================================================
//
//  [Pause.cpp]
// Author : 佐藤 颯紀
//
//=============================================================================

//=======================================================================================
// インクルードファイル
//=======================================================================================
#include "pause.h"
#include "scene2d.h"
#include "manager.h"
#include "keyboard.h"
#include "manager.h"
#include "renderer.h"
#include "fade.h"
#include "joypad.h"
#include "scene.h"
#include "uipause.h"
#include "sound.h"

//=======================================================================================
//静的メンバ変数宣言
//=======================================================================================
LPDIRECT3DTEXTURE9 CPause::m_pTexture[1] = {};
CUiPause *CPause::m_apPauselogo[MAX_PAUSE] = {};

//=======================================================================================
// タイトルクラスのコンストラクタ
//=======================================================================================
CPause::CPause(int nPriority) : CScene(nPriority)
{
	m_pScene = NULL;
	m_SelectPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=======================================================================================
// タイトルクラスのデストラクタ
//=======================================================================================
CPause::~CPause()
{

}

//=======================================================================================
// タイトルクラスのクリエイト処理
//=======================================================================================
CPause* CPause::Create(void)
{
	CPause* pPause = new CPause();

	pPause->Init(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
	
	return pPause;
}

//=======================================================================================
// タイトルクラスのテクスチャ読み込み処理
//=======================================================================================
HRESULT CPause::Load(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/pause001.png", 
		&m_pTexture[0]);

	return S_OK;
}

//=======================================================================================
// タイトルクラスの初期化処理
//=======================================================================================
HRESULT CPause::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	if (m_pScene == NULL)
	{
		m_pScene = CScene2d::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));

		//テクスチャの設定
		m_pScene->BindTexture(m_pTexture[0]);
	}

	if (m_apPauselogo[0] == NULL)
	{
		m_apPauselogo[0] = CUiPause::Create(D3DXVECTOR3(UIPAUSE_START_POS_X, UIPAUSE_START_POS_Y, 0.0f), D3DXVECTOR3(UIPAUSE_SIZE_X, UIPAUSE_SIZE_Y, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CUiPause::UIPAUSETYPE_START);
	}

	if (m_apPauselogo[1] == NULL)
	{
		m_apPauselogo[1] = CUiPause::Create(D3DXVECTOR3(UIPAUSE_TITLE_POS_X, UIPAUSE_TITLE_POS_Y, 0.0f), D3DXVECTOR3(UIPAUSE_SIZE_X, UIPAUSE_SIZE_Y, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), CUiPause::UIPAUSETYPE_TITLE);
	}

	m_SelectPos = D3DXVECTOR3(UIPAUSE_START_POS_X, UIPAUSE_START_POS_Y, 0.0f);

	//オブジェクトタイプの設定
	SetObjType(CScene::OBJTYPE_PAUSE);

	//フラグをtrueにする
	SetPause(true);
	return S_OK;
}

//=======================================================================================
// タイトルクラスの終了処理
//=======================================================================================
void CPause::Uninit(void)
{
	if (m_pScene != NULL)
	{
		if (m_pScene != NULL)
		{
			m_pScene->Uninit();
			m_pScene = NULL;
		}
	}

	for (int nCount = 0; nCount < MAX_PAUSE; nCount++)
	{
		if (m_apPauselogo[nCount] != NULL)
		{
			m_apPauselogo[nCount]->Uninit();
			m_apPauselogo[nCount] = NULL;
		}
	}

	//オブジェクトの破棄
	SetDeathFlag();
}

//=======================================================================================
// タイトルクラスの更新処理
//=======================================================================================
void CPause::Update(void)
{
	CInputKeyboard* pKey = CManager::GetKeyboard();
	CFade::FADE_MODE mode = CManager::GetFade()->GetFade();

	Select();
}

//=======================================================================================
// タイトルクラスの描画処理
//=======================================================================================
void CPause::Draw(void)
{

}

//=======================================================================================
// 選択処理
//=======================================================================================
void CPause::Select(void)
{
	CFade::FADE_MODE mode = CManager::GetFade()->GetFade();
	// ジョイパッドの取得
	DIJOYSTATE js = CInputJoypad::GetStick(0);

	//入力が存在する
	if (js.lX != 0.0f || js.lY != 0.0f)
	{
		if (js.lY < -5)
		{
			m_SelectPos = D3DXVECTOR3(UIPAUSE_START_POS_X, UIPAUSE_START_POS_Y, 0.0f);
		}

		if (js.lY > 5)
		{
			m_SelectPos = D3DXVECTOR3(UIPAUSE_TITLE_POS_X, UIPAUSE_TITLE_POS_Y, 0.0f);
		}
	}

	if (m_SelectPos.y == UIPAUSE_START_POS_Y)
	{
		if (CManager::GetJoypad()->GetJoystickTrigger(CInputJoypad::JOY_BUTTON_A, 0))
		{
			SetPause(false);
			Uninit();
			return;
		}
	}

	if (m_SelectPos.y == UIPAUSE_TITLE_POS_Y)
	{
		if (CManager::GetJoypad()->GetJoystickTrigger(CInputJoypad::JOY_BUTTON_A, 0))
		{
			SetPause(false);
			CFade *pFade = CManager::GetFade();
			pFade->SetFade(CManager::MODE_TYPE_TITLE);
			Uninit();
			return;
		}
	}
}