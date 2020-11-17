//=======================================================================================
//
// 背景描画処理 [bg.cpp]
// Author : Konishi Yuuto
//
//=======================================================================================

//=======================================================================================
// インクルードファイル
//=======================================================================================
#include "title.h"
#include "scene2d.h"
#include "manager.h"
#include "keyboard.h"
#include "manager.h"
#include "renderer.h"
#include "fade.h"
#include "titlelogo.h"
#include "sound.h"
#include "joypad.h"

//=======================================================================================
//静的メンバ変数宣言
//=======================================================================================
LPDIRECT3DTEXTURE9 CTitle::m_pTexture[1] = {};
CTitlelogo *CTitle::m_apTitlelogo[MAX_TITLE] = {};

//=======================================================================================
// タイトルクラスのコンストラクタ
//=======================================================================================
CTitle::CTitle(int nPriority) : CScene(nPriority)
{
	m_pScene = NULL;
	m_bModechenge = false;
}

//=======================================================================================
// タイトルクラスのデストラクタ
//=======================================================================================
CTitle::~CTitle()
{
	for (int nCount = 0; nCount < MAX_TITLE; nCount++)
	{
		m_apTitlelogo[nCount] = NULL;
	}
}

//=======================================================================================
// タイトルクラスのクリエイト処理
//=======================================================================================
CTitle* CTitle::Create(void)
{
	CTitle* pTitle = new CTitle();

	pTitle->Init(D3DXVECTOR3(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 0.0f), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));

	return pTitle;
}

//=======================================================================================
// タイトルクラスのテクスチャ読み込み処理
//=======================================================================================
HRESULT CTitle::Load(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/Texture/bg001.png", &m_pTexture[0]);
		return S_OK;
}

//=======================================================================================
// タイトルクラスの初期化処理
//=======================================================================================
HRESULT CTitle::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ビューポートの設定
	D3DVIEWPORT9 view_port;

	// ビューポートの左上座標
	view_port.X = 0;
	view_port.Y = 0;

	// ビューポートの幅
	view_port.Width = SCREEN_WIDTH;

	// ビューポートの高さ
	view_port.Height = SCREEN_HEIGHT;

	// ビューポート深度設定
	view_port.MinZ = 0.0f;
	view_port.MaxZ = 1.0f;

	// ビューポート設定
	if (FAILED(pDevice->SetViewport(&view_port)))
	{
		return E_FAIL;
	}

	if (m_pScene == NULL)
	{
		m_pScene = CScene2d::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));

		//テクスチャの設定
		m_pScene->BindTexture(m_pTexture[0]);
	}
	
	if (m_apTitlelogo[0] == NULL)
	{
		m_apTitlelogo[0] = CTitlelogo::Create(D3DXVECTOR3(TITLE_UI_POS_X, TITLE_UI_POS_Y, 0.0f), D3DXVECTOR3(TITLE_UI_SIZE, TITLE_UI_SIZE, 0.0f), CTitlelogo::LOGOTIPE_UI);
	}
	if (m_apTitlelogo[1] == NULL)
	{
		m_apTitlelogo[1] = CTitlelogo::Create(D3DXVECTOR3(TITLE_PLESS_POS_X, TITLE_PLESS_POS_Y, 0.0f), D3DXVECTOR3(TITLE_PLESS_SIZE_X, TITLE_PLESS_SIZE_Y, 0.0f), CTitlelogo::LOGOTIPE_PRESS);
	}
	if (m_apTitlelogo[2] == NULL)
	{
		m_apTitlelogo[2] = CTitlelogo::Create(D3DXVECTOR3(TITLE_POS_X, TITLE_POS_Y, 0.0f), D3DXVECTOR3(TITLE_SIZE_X, TITLE_SIZE_Y, 0.0f), CTitlelogo::LOGOTIPE_TITLE);
	}

	CSound *pSound = CManager::GetSound();
	pSound->Play(CSound::SOUND_LABEL_BGM_TITLE);

	return S_OK;
}

//=======================================================================================
// タイトルクラスの終了処理
//=======================================================================================
void CTitle::Uninit(void)
{

	for (int nCount = 0; nCount < TITLELOGO_TYPE; nCount++)
	{
		if (m_apTitlelogo[nCount] != NULL)
		{
			m_apTitlelogo[nCount]->Uninit();
			m_apTitlelogo[nCount] = NULL;
		}
	}

	if (m_pScene != NULL)
	{
		m_pScene->Uninit();
	}

	//BGMを止める処理
	CSound *pSound = CManager::GetSound();
	//pSound->Stop(CSound::SOUND_LABEL_BGM_TITLE);

	//オブジェクトの破棄
	SetDeathFlag();
}

//=======================================================================================
// タイトルクラスの更新処理
//=======================================================================================
void CTitle::Update(void)
{
	CInputKeyboard* pKey = CManager::GetKeyboard();
	CFade::FADE_MODE mode = CManager::GetFade()->GetFade();
	CSound *pSound = CManager::GetSound();
	
	// コントローラのstartを押したときか、エンターキーを押したとき
	if (CManager::GetJoypad()->GetJoystickTrigger(CInputJoypad::JOY_BUTTON_START, 0) && mode == CFade::FADE_MODE_NONE 
		|| pKey->GetTrigger(DIK_RETURN) && mode == CFade::FADE_MODE_NONE)
	{
		CFade *pFade = CManager::GetFade();
		pFade->SetFade(CManager::MODE_TYPE_TUTORIAL);
		pSound->Play(CSound::SOUND_LABEL_SE_START);
	}


	//エンターキーを押したとき
	/*if (pKey->GetTrigger(DIK_RETURN) && mode == CFade::FADE_MODE_NONE)
	{
		CFade *pFade = CManager::GetFade();
		pFade->SetFade(CManager::MODE_TYPE_TUTORIAL);
	}*/
}

//=======================================================================================
// タイトルクラスの描画処理
//=======================================================================================
void CTitle::Draw(void)
{

}