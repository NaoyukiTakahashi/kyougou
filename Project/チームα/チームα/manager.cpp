//=============================================================================
//
// マネージャー処理 [manager.cpp]
// Author : 
//
//=============================================================================

//=============================================================================
//インクルードファイル
//=============================================================================
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "keyboard.h"
#include "scene3D.h"
#include "billboard.h"
#include "model.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "fade.h"
#include "player.h"
#include "meshfield.h"
#include "bg.h"
#include "joypad.h"
#include "life.h"
#include "ui.h"
#include "charge.h"
#include "2d_explosion.h"
#include "beam.h"
#include "effect.h"
#include "particle.h"
#include "shock.h"
#include "bomb.h"
#include "lockon.h"
#include "explosion.h"
#include "smoke.h"
#include "sand.h"
#include "number.h"
#include "bill.h"
#include "uistart.h"
#include "titlelogo.h"
#include "uiend.h"
#include "sound.h"
#include "tutorial.h"
#include "splash.h"
#include "laser.h"
#include "missile.h"
#include "fire.h"
#include "sea.h"
#include "boost.h"
#include "pause.h"
#include "uipause.h"
#include "locus.h"

//=============================================================================
//静的メンバ変数宣言
//=============================================================================
CManager::MODE_TYPE CManager::m_mode = CManager::MODE_TYPE_TITLE;
CRenderer *CManager::m_pRenderer = NULL;	//レンダラークラスのポインタ変数
CInputKeyboard *CManager::m_pInput = NULL	;//入力処理クラスのポインタ変数
CConection *CManager::m_pConection = NULL;
CFade *CManager::m_pFade = NULL;
CTitle *CManager::m_pTitle = NULL;
CTutorial *CManager::m_pTutorial = NULL;
CGame *CManager::m_pGame = NULL;
CResult *CManager::m_pResult = NULL;
CInputJoypad *CManager::m_pJoypad = NULL;
CSound *CManager::m_pSound = NULL;			//サウンドクラスのポインタ
CScene *CManager::m_pScene = NULL;
//=============================================================================
//コンストラクタ
//=============================================================================
CManager::CManager()
{
}

//=============================================================================
//デストラクタ
//=============================================================================
CManager::~CManager()
{
}

//=============================================================================
//初期化処理
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	//レンダラークラスのインスタンス生成
	m_pRenderer = new CRenderer;

	//メモリが確保できたら
	if (m_pRenderer != NULL)
	{
		// 初期化処理
		if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
		{
			return -1;
		}
	}

	//入力処理クラスのインスタンス生成
	m_pInput = new CInputKeyboard;

	//メモリが確保できたら
	if (m_pInput != NULL)
	{
		if (FAILED(m_pInput->Init(hInstance, hWnd)))
		{
			return -1;
		}
	}

	//入力処理クラスのインスタンス生成
	m_pJoypad = new CInputJoypad;

	//メモリが確保できたら
	if (m_pJoypad != NULL)
	{
		if (FAILED(m_pJoypad->Init(hInstance, hWnd)))
		{
			return -1;
		}
	}

	//サウンドのインスタンス生成
	m_pSound = new CSound;
	if (m_pSound != NULL)
	{
		m_pSound->Init(hWnd);
	}

	//タイトルクラスのクリエイト
	//m_pTitle = CTitle::Create();

	//フェードクラスのクリエイト
	m_pFade = CFade::Create();

	//全テクスチャの読み込み
	LoadAll();

	return S_OK;
}

//=============================================================================
//終了処理
//=============================================================================
void CManager::Uninit(void)
{
	//全テクスチャの破棄
	UnLoadAll();
	
	if (m_pFade != NULL)
	{
		//フェードクラスの終了処理呼び出し
		m_pFade->Uninit();

		//メモリの破棄
		delete m_pFade;

		//メモリのクリア
		m_pFade = NULL;
	}

	if (m_pInput != NULL)
	{
		//入力処理クラスの終了処理呼び出し
		m_pInput->Uninit();

		//メモリの削除
		delete m_pInput;

		//メモリのクリア
		m_pInput = NULL;
	}

	if (m_pJoypad != NULL)
	{
		//入力処理クラスの終了処理呼び出し
		m_pJoypad->Uninit();

		//メモリの削除
		delete m_pInput;

		//メモリのクリア
		m_pJoypad = NULL;
	}

	if (m_pRenderer != NULL)
	{
		//レンダラークラスの終了処理呼び出し
		m_pRenderer->Uninit();

		//メモリの削除
		delete m_pRenderer;

		//メモリのクリア
		m_pRenderer = NULL;
	}
}

//=============================================================================
//更新処理
//=============================================================================
void CManager::Update(void)
{
	if (m_pInput != NULL)
	{
		//入力処理クラスの更新処理呼び出し
		m_pInput->Update();
	}

	if (m_pJoypad != NULL)
	{
		//入力処理クラスの更新処理呼び出し
		m_pJoypad->Update();
	}

	if (m_pRenderer != NULL)
	{
		//レンダラークラスの更新処理呼び出し
		m_pRenderer->Update();
	}

	if (m_pFade != NULL)
	{
		//フェードクラスの更新処理呼び出し
		m_pFade->Update();
	}

	/*switch (m_mode)
	{
	case MODE_TYPE_GAME:
		if (CManager::GetJoypad()->GetJoystickTrigger(CInputJoypad::JOY_BUTTON_START, 0))
		{
			m_pScene->GetPause(false);
		}

		else if (CManager::GetJoypad()->GetJoystickTrigger(CInputJoypad::JOY_BUTTON_B, 0))
		{
			m_pScene->GetPause(true);
		}
		break;
	}*/
}

//=============================================================================
//描画処理
//=============================================================================
void CManager::Draw(void)
{
	if (m_pRenderer != NULL)
	{
		//レンダラークラスの描画処理呼び出し
		m_pRenderer->Draw();
	}
}

//=============================================================================
//テクスチャの全ロード処理
//=============================================================================
void CManager::LoadAll(void)
{
	CTitle::Load();
	CResult::Load();
	CMeshField::Load();
	CBg::Load();	
	CLife::Load();
	CUi::Load();
	CCharge::Load();
	CLife::Load();
	C2dExplosion::Load();
	CBeam::Load();
	CEffect::Load();
	CParticle::Load();
	CShock::Load();
	CBomb::Load();
	CLockon::Load();
	CNumber::Load();
	CBill::LoadModel();
	CUiStart::Load();
	CTitlelogo::Load();
	CExplosion::Load();
	CSmoke::Load();
	CSand::Load();
	CUiEnd::Load();
	CTutorial::Load();
	CBill::LoadModel();
	CSplash::Load();
	CLaser::Load();
	CLocus::Load();
	CFire::Load();
	CBoost::Load();
	CSea::Load();
	CPause::Load();
	CUiPause::Load();
}

//=============================================================================
//テクスチャの全アンロード処理
//=============================================================================
void CManager::UnLoadAll(void)
{
	CMeshField::UnLoad();
	CBg::UnLoad();
	CLife::Unload();
	CUi::Unload();
	CCharge::Unload();	
	C2dExplosion::UnLoad();
	CBeam::UnLoad();
	CEffect::UnLoad();
	CParticle::UnLoad();
	CShock::UnLoad();
	CBomb::UnLoad();
	CLockon::Unload();
	CBill::Unload();
	CUiStart::Unload();
	CTitlelogo::Unload();
	CExplosion::UnLoad();
	CSmoke::UnLoad();
	CSand::UnLoad();
	CSplash::UnLoad();
	CLaser::UnLoad();

	CFire::UnLoad();
	CBoost::UnLoad();
	CUiPause::UnLoad();
	CLockon::Unload();
	CNumber::Unload();
	CUiEnd::Unload();
	CSea::UnLoad();
	CLocus::UnLoad();
}

//=============================================================================
//ゲームモードの設定処理
//=============================================================================
void CManager::SetMode(MODE_TYPE mode)
{
	CSound *pSound = CManager::GetSound();

	//現在モードの終了
	switch (m_mode)
	{
	case MODE_TYPE_TITLE:
		if (m_pTitle != NULL)
		{
			m_pTitle->Uninit();

			m_pTitle = NULL;
		}
		break;

	case MODE_TYPE_TUTORIAL:
		if (m_pTutorial != NULL)
		{
			m_pTutorial->Uninit();
			pSound->Stop(CSound::SOUND_LABEL_BGM_TITLE);
			m_pTutorial = NULL;
		}
		break;

	case MODE_TYPE_GAME:
		if (m_pGame != NULL)
		{
			m_pGame->Uninit();
			pSound->Stop(CSound::SOUND_LABEL_BGM_GAME);
			m_pGame = NULL;
		}
		break;

	case MODE_TYPE_RESULT:
		if (m_pResult != NULL)
		{
			m_pResult->Uninit();

			m_pResult = NULL;
		}
		break;

	}

	CScene::ReleaseAll();

	//モードを設定
	m_mode = mode;

	//設定されたモードをクリエイト
	switch (m_mode)
	{
	case MODE_TYPE_TITLE:
		if (m_pTitle == NULL)
		{
			m_pTitle = CTitle::Create();
		}
		
		break;

	case MODE_TYPE_TUTORIAL:
		if (m_pTutorial == NULL)
		{
			m_pTutorial = CTutorial::Create();
		}

		break;

	case MODE_TYPE_GAME:
		if (m_pGame == NULL)
		{
			m_pGame = CGame::Create();
		}

		break;

	case MODE_TYPE_RESULT:
		if (m_pGame == NULL)
		{
			m_pResult = CResult::Create();
		}

	default:
		break;
	}
}

//=============================================================================
//ゲームモード情報の取得
//=============================================================================
CManager::MODE_TYPE CManager::GetMode(void)
{
	return m_mode;
}

//=============================================================================
//レンダラー情報取得
//=============================================================================
CRenderer * CManager::GetRenderer(void)
{
	return m_pRenderer;
}


//=============================================================================
//キーボード情報取得
//=============================================================================
CInputKeyboard * CManager::GetKeyboard(void)
{
	return m_pInput;
}

CConection *CManager::GetConection(void)
{
	return m_pConection;
}

//=============================================================================
//フェード情報取得
//=============================================================================
CFade * CManager::GetFade(void)
{
	return m_pFade;
}

//=============================================================================
//ジョイパッド情報取得
//=============================================================================
CInputJoypad * CManager::GetJoypad(void)
{
	return m_pJoypad;
}

//=============================================================================
//ジョイパッド情報取得
//=============================================================================
CSound * CManager::GetSound(void)
{
	return m_pSound;
}
