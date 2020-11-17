//=======================================================================================
//
// ゲーム処理 [game.cpp]
// Author : Konishi Yuuto
//
//=======================================================================================

//=======================================================================================
// インクルード
//=======================================================================================
#include "game.h"
#include "camera.h"
#include "light.h"
#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "meshfield.h"
#include "shape.h"
#include "bg.h"
#include "joypad.h"
#include "time.h"
#include "gauge.h"
#include "life.h"
#include "ui.h"
#include "lockon.h"
#include "charge.h"
#include "uistart.h"
#include "fire.h"
#include "bill.h"
#include "continue.h"
#include "uiend.h"
#include "sound.h"
#include "pause.h"
#include "keyboard.h"
#include "missile.h"
#include "sea.h"


//=======================================================================================
// static初期化
//=======================================================================================
CCamera *CGame::m_apCamera[MAX_PLAYER] = {};		// カメラクラスのポインタ変数
CLight *CGame::m_pLight = NULL;						// ライトクラスのポインタ変数
CMeshField *CGame::m_pMeshField = NULL;				// メッシュフィールド
CMeshShape *CGame::m_pSphere = NULL;				// メッシュスフィア
CBg *CGame::m_pBg = NULL;							// 背景のポインタ
CPlayer *CGame::m_apPlayer[MAX_PLAYER] = {};		// プレイヤーのポインタ
CTime *CGame::m_pTime = NULL;						// タイムのポインタ
CUi *CGame::m_pUi = NULL;							// uiのポインタ
CUiStart *CGame::m_pUiStart = NULL;
CLockon *CGame::m_pLockon = NULL;
CBill *CGame::m_pBill = NULL;
CContinue *CGame::m_pContinue = NULL;
CUiEnd *CGame::m_pUiEnd = NULL;
CPause *CGame::m_pPause = NULL;
int CGame::m_nRoundNum = 0;
int CGame::m_aWinNum[MAX_PLAYER] = {};
CSea *CGame::m_pSea = NULL;

//=======================================================================================
// コンストラクタ
//=======================================================================================
CGame::CGame(int nPriority) : CScene(nPriority)
{
	m_bGameEnd = false;
}

//=======================================================================================
// デストラクタ
//=======================================================================================
CGame::~CGame()
{
	m_nRoundNum = 0;
}

//=======================================================================================
// クリエイト
//=======================================================================================
CGame* CGame::Create(void)
{
	// メモリ確保
	CGame* pGame = new CGame();

	// 初期化処理
	pGame->Init(D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	return pGame;
}

//=======================================================================================
// 初期化処理
//=======================================================================================
HRESULT CGame::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	m_nRoundNum = 0;

	CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	for (int nCount = 0; nCount < MAX_PLAYER; nCount++)
	{
		//カメラクラスのクリエイト
		m_apCamera[nCount] = CCamera::Create();
	}

	//ライトクラスの生成
	m_pLight = new CLight;

	// ライトの初期化処理
	if (m_pLight != NULL)
	{
		if (FAILED(m_pLight->Init()))
		{
			return -1;
		}
	}

	//ui
	if (m_pUi == NULL)
	{

		//UIライフゲージ(外枠)の生成

		m_pUi = CUi::Create(D3DXVECTOR3(330.0f, 30.0f, 0.0f), D3DXVECTOR3(UI_LIFE_SIZE_X / 2, UI_LIFE_SIZE_PLAYERY, 0.0f), CUi::UITTYPE_LIFE);
		m_pUi = CUi::Create(D3DXVECTOR3(1060.0f, 30.0f, 0.0f), D3DXVECTOR3(UI_LIFE_SIZE_X / 2, UI_LIFE_SIZE_PLAYERY, 0.0f), CUi::UITTYPE_LIFE);

		m_pUi = CUi::Create(D3DXVECTOR3(330.0f, 65.0f, 0.0f), D3DXVECTOR3(UI_LIFE_SIZE_X / 2, UI_LIFE_SIZE_ENEMYY, 0.0f), CUi::UITTYPE_LIFE);
		m_pUi = CUi::Create(D3DXVECTOR3(1060.0f, 65.0f, 0.0f), D3DXVECTOR3(UI_LIFE_SIZE_X / 2, UI_LIFE_SIZE_ENEMYY, 0.0f), CUi::UITTYPE_LIFE);
		//UIのタイム生成
		m_pUi = CUi::Create(D3DXVECTOR3(640.0f, 50.0f, 0.0f), D3DXVECTOR3(UI_TIME_SIZE_X, UI_TIME_SIZE_Y, 0.0f), CUi::UITYPE_TIME);

		//UIのプレイヤー・エネミー文字の生成
		m_pUi = CUi::Create(D3DXVECTOR3(70.0f, 30.0f, 0.0f), D3DXVECTOR3(UI_PLAYER_SIZE_X, UI_PLAYER_SIZE_Y, 0.0f), CUi::UITYPE_PLAYER);
		m_pUi = CUi::Create(D3DXVECTOR3(75.0f, 65.0f, 0.0f), D3DXVECTOR3(UI_ENEMY_SIZE_X, UI_ENEMY_SIZE_Y, 0.0f), CUi::UITYPE_ENEMY);

		m_pUi = CUi::Create(D3DXVECTOR3(800.0f, 30.0f, 0.0f), D3DXVECTOR3(UI_PLAYER_SIZE_X, UI_PLAYER_SIZE_Y, 0.0f), CUi::UITYPE_PLAYER);
		m_pUi = CUi::Create(D3DXVECTOR3(805.0f, 65.0f, 0.0f), D3DXVECTOR3(UI_ENEMY_SIZE_X, UI_ENEMY_SIZE_Y, 0.0f), CUi::UITYPE_ENEMY);

		//標準生成
		m_pUi = CUi::Create(D3DXVECTOR3(UI_LOCKON_POS_LEFT_X, UI_LOCKON_POS_Y, 0.0f), D3DXVECTOR3(UI_LOCKON_SIZE_SMALL_X, UI_LOCKON_SIZE_SMALL_Y, 0.0f), CUi::UITYPE_STANDARD);
		m_pUi = CUi::Create(D3DXVECTOR3(UI_LOCKON_POS_RIGHT_X, UI_LOCKON_POS_Y, 0.0f), D3DXVECTOR3(UI_LOCKON_SIZE_SMALL_X, UI_LOCKON_SIZE_SMALL_Y, 0.0f), CUi::UITYPE_STANDARD);
	}

	//startのUI
	if (m_pUiStart == NULL)
	{
		//ready文字
		m_pUiStart = CUiStart::Create(D3DXVECTOR3(UI_READY_POS_LEFT_X, UI_READY_POS_Y, 0.0f), D3DXVECTOR3(UI_READY_SIZE_X, UI_READY_SIZE_Y, 0.0f), CUiStart::UISTARTTYPE_READY);
		m_pUiStart = CUiStart::Create(D3DXVECTOR3(UI_READY_POS_RIGHT_X, UI_READY_POS_Y, 0.0f), D3DXVECTOR3(UI_READY_SIZE_X, UI_READY_SIZE_Y, 0.0f), CUiStart::UISTARTTYPE_READY);
		//round文字
		m_pUiStart = CUiStart::Create(D3DXVECTOR3(UI_ROUND_POS_LEFT_X, UI_ROUND_POS_Y, 0.0f), D3DXVECTOR3(UI_ROUND_SIZE_X, UI_ROUND_SIZE_Y, 0.0f), CUiStart::UISTARTTYPE_ROUND_1);
		m_pUiStart = CUiStart::Create(D3DXVECTOR3(UI_ROUND_POS_RIGHT_X, UI_ROUND_POS_Y, 0.0f), D3DXVECTOR3(UI_ROUND_SIZE_X, UI_ROUND_SIZE_Y, 0.0f), CUiStart::UISTARTTYPE_ROUND_1);
	}

	// プレイヤーの生成
	if (m_apPlayer[0] == NULL)
	{
		m_apPlayer[0] = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, -1000.0f), D3DXVECTOR3(PLAYER_SIZE_X, PLAYER_SIZE_Y, PLAYER_SIZE_Z));	
	}
	if (m_apPlayer[1] == NULL)
	{
		m_apPlayer[1] = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 1000.0f), D3DXVECTOR3(PLAYER_SIZE_X, PLAYER_SIZE_Y, PLAYER_SIZE_Z));
	}

	//ビル
	if (m_pBill == NULL)
	{
		m_pBill = CBill::Create(D3DXVECTOR3(BILL_POS_X_1, 0.0f, -BILL_POS_Z), D3DXVECTOR3(BILL_SIZE_X, BILL_SIZE_Y, BILL_SIZE_Z));
		m_pBill = CBill::Create(D3DXVECTOR3(-BILL_POS_X_1, 0.0f, -BILL_POS_Z), D3DXVECTOR3(BILL_SIZE_X, BILL_SIZE_Y, BILL_SIZE_Z));
		m_pBill = CBill::Create(D3DXVECTOR3(BILL_POS_X_1, 0.0f, BILL_POS_Z), D3DXVECTOR3(BILL_SIZE_X, BILL_SIZE_Y, BILL_SIZE_Z));
		m_pBill = CBill::Create(D3DXVECTOR3(-BILL_POS_X_1, 0.0f, BILL_POS_Z), D3DXVECTOR3(BILL_SIZE_X, BILL_SIZE_Y, BILL_SIZE_Z));
		m_pBill = CBill::Create(D3DXVECTOR3(BILL_POS_X_2, 0.0f, 0.0f), D3DXVECTOR3(BILL_SIZE_X, BILL_SIZE_Y, BILL_SIZE_Z));
		m_pBill = CBill::Create(D3DXVECTOR3(-BILL_POS_X_2, 0.0f, .0f), D3DXVECTOR3(BILL_SIZE_X, BILL_SIZE_Y, BILL_SIZE_Z));
	}

	
	// メッシュフィールド
	if (m_pMeshField == NULL)
	{
		m_pMeshField = CMeshField::Create();
	}

//	CMeshShape::Load();

	// 海
	if (m_pSea == NULL)
	{
		// 生成
		m_pSea = CSea::Create();
	}

	//// メッシュスフィア
	//if (m_pSphere == NULL)
	//{
	//	m_pSphere = CMeshShape::Create();
	//}

	// 背景
	if (m_pBg == NULL)
	{
		m_pBg = CBg::Create();
	}

	//タイム
	if (m_pTime == NULL)
	{
		
		m_pTime = CTime::Create(D3DXVECTOR3(TIME_POS_X, TIME_POS_Y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	//BGM
	CSound *pSound = CManager::GetSound();
	pSound->Play(CSound::SOUND_LABEL_BGM_GAME);

	//デバイス情報の取得
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	//フォントの生成
	D3DXCreateFont(pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont);

	return S_OK;
}

//=======================================================================================
// 終了処理
//=======================================================================================
void CGame::Uninit(void)
{
	for (int nCount = 0; nCount < MAX_PLAYER; nCount++)
	{
		if (m_apCamera[nCount] != NULL)
		{
			//カメラクラスの終了処理呼び出す
			m_apCamera[nCount]->Uninit();

			//メモリの破棄
			delete m_apCamera[nCount];

			//メモリのクリア
			m_apCamera[nCount] = NULL;
		}
	}

	// メッシュフィールド
	if (m_pMeshField != NULL)
	{
		m_pMeshField->Uninit();
		m_pMeshField = NULL;
	}

	// 海
	if (m_pSea != NULL)
	{
		m_pSea->Uninit();
		m_pSea = NULL;
	}

	//CMeshShape::UnLoad();
	//// メッシュスフィア
	//if (m_pSphere != NULL)
	//{
	//	m_pSphere->Uninit();
	//}

	// 背景
	if (m_pBg != NULL)
	{
		m_pBg->Uninit();
		m_pBg = NULL;
	}

	// ライトの終了処理
	if (m_pLight != NULL)
	{
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = NULL;
	}

	// プレイヤー終了処理
	for (int nCount = 0; nCount < MAX_PLAYER; nCount++)
	{
		if (m_apPlayer[nCount] != NULL)
		{
			m_apPlayer[nCount]->Uninit();
			m_apPlayer[nCount] = NULL;
		}
	}

	// タイム
	if (m_pTime != NULL)
	{
		m_pTime->Uninit();
		m_pTime = NULL;
	}

	// UI
	if (m_pUi != NULL)
	{
		m_pUi = NULL;
	}

	// スタートUI
	if (m_pUiStart != NULL)
	{
		m_pUiStart = NULL;
	}

	// ビル
	if (m_pBill != NULL)
	{
		m_pBill = NULL;
	}

	// コンティニュー
	if (m_pContinue != NULL)
	{
		m_pContinue->Uninit();
		m_pContinue = NULL;
	}

	m_nRoundNum = 0;

	for (int nCount = 0; nCount < MAX_PLAYER; nCount++)
	{
		m_aWinNum[nCount] = 0;
	}

	//サウンド情報取得
	CSound *pSound = CManager::GetSound();

	//ゲームBGM停止
	pSound->Stop(CSound::SOUND_LABEL_BGM_GAME);

	//オブジェクトの破棄
	SetDeathFlag();
}

//=======================================================================================
// 更新処理
//=======================================================================================
void CGame::Update(void)
{
	for (int nCount = 0; nCount < MAX_PLAYER; nCount++)
	{
		if (m_apCamera[nCount] != NULL)
		{
			//カメラクラスの更新処理

			m_apCamera[nCount]->Update();
		}
	}

	// メッシュフィールド
	if (m_pMeshField != NULL)
	{
		m_pMeshField->Update();
	}

	// 海
	if (m_pSea != NULL)
	{
		m_pSea->Update();
	}

	// メッシュスフィア
	if (m_pSphere != NULL)
	{
		m_pSphere->Update();
	}

	//for (int nCount = 0; nCount < FIRE_NUM; nCount++)
	//{
	//	// 炎の生成
	//	CFire::Create(D3DXVECTOR3(0.0f, 50.0f, 0.0f),
	//		D3DXVECTOR3(FIRE_SIZE_X, FIRE_SIZE_Y, 0.0f), FIRE_LIFE);
	//}

	//コンテニューのタイム
	if (m_pContinue != NULL)
	{
		if (m_pContinue->GetEnd() == true)
		{
			m_pContinue->Uninit();
			m_pContinue = NULL;
		}
	}
	CInputKeyboard* pKey = CManager::GetKeyboard();

	if (CManager::GetJoypad()->GetJoystickTrigger(CInputJoypad::JOY_BUTTON_START, 0) || pKey->GetTrigger(DIK_I))
	{
		if (m_bGameEnd == false)
		{
			m_pPause = CPause::Create();
		}
	}
	// ゲームの設定
	SetGame();
}

//=======================================================================================
// 描画処理
//=======================================================================================
void CGame::Draw(void)
{
	// メッシュフィールド
	if (m_pMeshField != NULL)
	{
		m_pMeshField->Draw();
	}

	//// メッシュスフィア
	//if (m_pSphere != NULL)
	//{
	//	m_pSphere->Draw();
	//}

	// 海
	if (m_pSea != NULL)
	{
		m_pSea->Draw();
	}

	// 背景
	if (m_pBg != NULL)
	{
		m_pBg->Draw();
	}
}

//=======================================================================================
// ゲームの設定
//=======================================================================================
void CGame::SetGame(void)
{
	float SizeX[MAX_PLAYER] = { 0.0f, 0.0f };
	
	for (int nCount = 0; nCount < MAX_PLAYER; nCount++)
	{
		// プレイヤーの情報取得
		CPlayer *pPlayer = GetPlayer(nCount);
		bool bEnd = pPlayer->GetEnd();

		// どちらかが終わっていたら
		if (bEnd == true)
		{
			switch (nCount)
			{
			case 0:
				if (pPlayer->GetDraw() == false)
				{
					m_aWinNum[1]++;

					if (m_aWinNum[1] == 1)
					{
						CUi::Create(D3DXVECTOR3(UI_WINMARK_POS_RIGHT1_X, UI_WINMARK_POS_Y, 0.0f), D3DXVECTOR3(UI_WINMARK_SIZE_X, UI_WINMARK_SIZE_Y, 0.0f), CUi::UYTYPE_WINMARK);
					}
					else if (m_aWinNum[2] == 2)
					{
						CUi::Create(D3DXVECTOR3(UI_WINMARK_POS_RIGHT2_X, UI_WINMARK_POS_Y, 0.0f), D3DXVECTOR3(UI_WINMARK_SIZE_X, UI_WINMARK_SIZE_Y, 0.0f), CUi::UYTYPE_WINMARK);
					}
				}
				break;

			case 1:
				if (pPlayer->GetDraw() == false)
				{
					m_aWinNum[0]++;

					if (m_aWinNum[0] == 1)
					{
						CUi::Create(D3DXVECTOR3(UI_WINMARK_POS_LEFT1_X, UI_WINMARK_POS_Y, 0.0f), D3DXVECTOR3(UI_WINMARK_SIZE_X, UI_WINMARK_SIZE_Y, 0.0f), CUi::UYTYPE_WINMARK);
					}
					else if (m_aWinNum[0] == 2)
					{
						CUi::Create(D3DXVECTOR3(UI_WINMARK_POS_LEFT2_X, UI_WINMARK_POS_Y, 0.0f), D3DXVECTOR3(UI_WINMARK_SIZE_X, UI_WINMARK_SIZE_Y, 0.0f), CUi::UYTYPE_WINMARK);

					}


				}
				break;

			default:
				break;
			}

			if (m_aWinNum[0] < 2 && m_aWinNum[1] < 2)
			{
				// ラウンドを進める
				m_nRoundNum++;

				// 初期化処理
				ResetGame();
			}
			else
			{
				if (m_bGameEnd == false)
				{
					//コンテニューのタイム
					if (m_pContinue == NULL)
					{
						m_pContinue = CContinue::Create(D3DXVECTOR3(CONTINUE_POS_X, CONTINUE_POS_Y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
						m_bGameEnd = true;
					}
				}
			}
			break;
		}
	}
}

//=======================================================================================
// ゲームのリセット関数
//=======================================================================================
void CGame::ResetGame(void)
{
	if (m_apPlayer[0] != NULL)
	{
		m_apPlayer[0]->Init(D3DXVECTOR3(0.0f, 0.0f, -500.0f), D3DXVECTOR3(PLAYER_SIZE_X, PLAYER_SIZE_Y, PLAYER_SIZE_Z));

		// ロゴの終了
		m_apPlayer[0]->SetWinToLose(false);
	}
	if (m_apPlayer[1] != NULL)
	{
		m_apPlayer[1]->Init(D3DXVECTOR3(PLAYER2_POS_X, 0.0f, PLAYER2_POS_Z), D3DXVECTOR3(PLAYER_SIZE_X, PLAYER_SIZE_Y, PLAYER_SIZE_Z));
		m_apPlayer[1]->SetWinToLose(false);
	}

	//タイム
	if (m_pTime != NULL)
	{
		m_pTime->Init(D3DXVECTOR3(TIME_POS_X, TIME_POS_Y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	switch (m_nRoundNum)
	{
	case 1:
		//ready文字
		m_pUiStart = CUiStart::Create(D3DXVECTOR3(UI_READY_POS_LEFT_X, UI_READY_POS_Y, 0.0f), D3DXVECTOR3(UI_READY_SIZE_X, UI_READY_SIZE_Y, 0.0f), CUiStart::UISTARTTYPE_READY);
		m_pUiStart = CUiStart::Create(D3DXVECTOR3(UI_READY_POS_RIGHT_X, UI_READY_POS_Y, 0.0f), D3DXVECTOR3(UI_READY_SIZE_X, UI_READY_SIZE_Y, 0.0f), CUiStart::UISTARTTYPE_READY);
		//round文字
		m_pUiStart = CUiStart::Create(D3DXVECTOR3(UI_ROUND_POS_LEFT_X, UI_ROUND_POS_Y, 0.0f), D3DXVECTOR3(UI_ROUND_SIZE_X, UI_ROUND_SIZE_Y, 0.0f), CUiStart::UISTARTTYPE_ROUND_2);
		m_pUiStart = CUiStart::Create(D3DXVECTOR3(UI_ROUND_POS_RIGHT_X, UI_ROUND_POS_Y, 0.0f), D3DXVECTOR3(UI_ROUND_SIZE_X, UI_ROUND_SIZE_Y, 0.0f), CUiStart::UISTARTTYPE_ROUND_2);

		break;
	case 2:
		//ready文字
		m_pUiStart = CUiStart::Create(D3DXVECTOR3(UI_READY_POS_LEFT_X, UI_READY_POS_Y, 0.0f), D3DXVECTOR3(UI_READY_SIZE_X, UI_READY_SIZE_Y, 0.0f), CUiStart::UISTARTTYPE_READY);
		m_pUiStart = CUiStart::Create(D3DXVECTOR3(UI_READY_POS_RIGHT_X, UI_READY_POS_Y, 0.0f), D3DXVECTOR3(UI_READY_SIZE_X, UI_READY_SIZE_Y, 0.0f), CUiStart::UISTARTTYPE_READY);
		//round文字
		m_pUiStart = CUiStart::Create(D3DXVECTOR3(UI_ROUND_POS_LEFT_X, UI_ROUND_POS_Y, 0.0f), D3DXVECTOR3(UI_ROUND_SIZE_X, UI_ROUND_SIZE_Y, 0.0f), CUiStart::UISTARTTYPE_ROUND_3);
		m_pUiStart = CUiStart::Create(D3DXVECTOR3(UI_ROUND_POS_RIGHT_X, UI_ROUND_POS_Y, 0.0f), D3DXVECTOR3(UI_ROUND_SIZE_X, UI_ROUND_SIZE_Y, 0.0f), CUiStart::UISTARTTYPE_ROUND_3);

		break;
	default:
		break;

	}
}

//=======================================================================================
// カメラの情報
//=======================================================================================
CCamera * CGame::GetCamera(int nCount)
{
	return m_apCamera[nCount];
}

//=======================================================================================
// ライトの情報
//=======================================================================================
CLight * CGame::GetLight(void)
{
	return m_pLight;
}

//=======================================================================================
// プレイヤーの情報
//=======================================================================================
CPlayer * CGame::GetPlayer(int nCount)
{
	return m_apPlayer[nCount];
}

//=======================================================================================
// タイムの情報
//=======================================================================================
CTime * CGame::GetTime(void)
{
	return m_pTime;
}

//=======================================================================================
// uiの情報
//=======================================================================================
CUi * CGame::GetUi(void)
{
	return m_pUi;
}


//=======================================================================================
// uistartの情報
//=======================================================================================
CUiStart * CGame::GetUiStart(void)
{
	return m_pUiStart;
}

//=======================================================================================
// uiendの情報
//=======================================================================================
CUiEnd *CGame::GetUiEnd(void)
{
	return m_pUiEnd;
}

CMeshShape * CGame::GetSphere()
{
	return m_pSphere;
}

//=======================================================================================
// ロックオンの情報
//=======================================================================================
CLockon * CGame::GetLockon(void)
{
	
	return m_pLockon;
}

//=======================================================================================
// ビルの情報
//=======================================================================================
CBill * CGame::GetBill(void)
{
	return m_pBill;
}

//=======================================================================================
// コンテニューの情報
//=======================================================================================
CContinue * CGame::GetContinue(void)
{
	return m_pContinue;
}

//=======================================================================================
// コンテニューの情報
//=======================================================================================
CPause * CGame::GetPause(void)
{
	return m_pPause;
}
