//=============================================================================
//
// メイン処理 [camera.cpp]
// Author : 高橋那伯志
//
//=============================================================================
#include "camera.h"
#include "manager.h"
#include "input.h"
#include "renderer.h"
#include "keyboard.h"
#include "game.h"
#include "player.h"
#include "joypad.h"
#include "lockon.h"
#include "ui.h"
#include "life.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define CAMERA_DEFAULT_Fθ			(D3DXToRadian(75.0f))	//カメラのθDefault値
#define DISTANCE					(1300.0f)				//視点～注視点の距離
#define PLAYER_HEIGHT				(200.0f)				//注視点の高さ
#define PLAYE_ROT_VERTICAL_FRONT	(D3DXToRadian(0.0f))	//プレイヤーの縦軸前
#define WIN_START_ROT_VERTICAL		(D3DXToRadian(180.0f))	//勝利時縦軸回転の開始角
#define WIN_START_ROT_HORIZONTAL	(D3DXToRadian(40.0f))	//勝利時横軸回転の開始角
#define WIN_START_DISTANCE			(1500.0f)				//勝利時の開始距離

#define WIN_END_ROT_VERTICAL		(D3DXToRadian(360.0f+180.0f))	//勝利時縦軸回転の終了角
#define WIN_END_ROT_HORIZONTAL		(D3DXToRadian(100.0f))	//勝利時横軸回転の終了角
#define WIN_END_DISTANCE			(500.0f)				//勝利時の終了距離
#define WIN_ROT_MOVE_SUBTRACTION	(0.2f)					//勝利時の横軸回転角度変化量

#define LOSE_START_ROT_VERTICAL		(D3DXToRadian(180.0f))	//敗北時縦軸回転の開始角
#define LOSE_START_ROT_HORIZONTAL	(D3DXToRadian(60.0f))	//敗北時横軸回転の開始角
#define LOSE_START_DISTANCE			(500.0f)				//敗北時の開始距離
#define LOSE_END_DISTANCE			(1000.0f)				//敗北時の終了距離
#define LOSE_MOVE_SUBTRACTION		(0.2f)					//敗北時の横軸回転角度変化量

#define CAMERA_MIN_Fφ				(D3DXToRadian(10.0f))	//カメラの最小角
#define CAMERA_MAX_Fφ				(D3DXToRadian(170.0f))	//カメラの最大角
#define CAMERA_MIN_HIGHT			(2.0f)					//カメラの最低高度
#define STICK_SENSITIVITY			(50.0f)					//スティック感度
#define STICK_INPUT_CONVERSION		(D3DXToRadian(1.0f))	//スティック入力変化量
#define JUDGMENT_RANGE				(50.0f)					//判定距離

//=============================================================================
// static初期化
//=============================================================================
int CCamera::m_nCameraAll = 0;

//=============================================================================
// クリエイト
//=============================================================================
CCamera * CCamera::Create(void)
{
	// 初期化処理
	CCamera *pCamera = new CCamera;

	pCamera->Init();

	return pCamera;
}

//=============================================================================
//カメラクラスのコンストラクタ
//=============================================================================
CCamera::CCamera()
{
	//各メンバ変数のクリア
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// カメラの座標
	m_posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// カメラの座標（目的地）
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 注視点
	m_posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 注視点（目的地）
	m_posU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 上方向ベクトル
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き
	m_fDistance = 0.0f;							// 視点～注視点の距離
	m_fMove = 0.0f;								// 移動量
	m_nCameraNum = m_nCameraAll++;				// カメラの番号
	m_bTarget = false;							//ターゲットoff
	m_state = CAMERASTATE_NONE;					//ステータス初期化
	memset(m_pLockon, 0, sizeof(m_pLockon));
	m_nWinPlayer = -1;							//Winプレイヤー番号
	m_fDifference = 0;	//勝利カメラ回転開始角から終了角の差
	m_fEndVertical = 0;	//勝利カメラ終了角度
}

//=============================================================================
//カメラクラスのデストラクタ
//=============================================================================
CCamera::~CCamera()
{
}

//=============================================================================
//カメラクラスの初期化処理
//=============================================================================
HRESULT CCamera::Init(void)
{
	// レンダラーの情報を受け取る
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	switch (m_nCameraNum)
	{
	case 0:
		m_fMove = 5.0f;
		m_fDistance = DISTANCE;
		m_rot.y = 0.0f;
		m_fθ = CAMERA_DEFAULT_Fθ;	
		m_fφ = D3DXToRadian(180.0f);						// 初期値敵のほう向ける
		m_posR = D3DXVECTOR3(0.0f, PLAYER_HEIGHT, 0.0f);	// 注視点設定
		m_posU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);				// 上方向ベクトル
		m_posV.x = m_posR.x + m_fDistance* sinf(m_fθ) * sinf(m_fφ);	//カメラ位置X
		m_posV.y = m_posR.z + m_fDistance* cosf(m_fθ);					//カメラ位置Y
		m_posV.z = m_posR.y + m_fDistance* sinf(m_fθ) * cosf(m_fφ);	//カメラ位置Z
		break;

	case 1:
		m_fMove = 5.0f;
		m_fDistance = DISTANCE;
		m_fθ = CAMERA_DEFAULT_Fθ;
		m_fφ = D3DXToRadian(0.0f);									// 初期値敵のほう向ける
		m_posR = D3DXVECTOR3(0.0f, PLAYER_HEIGHT, 0.0f);			// 注視点設定
		m_posU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);						// 上方向ベクトル
		m_posV.x = m_posR.x + m_fDistance* sinf(m_fθ) * sinf(m_fφ);	//カメラ位置X
		m_posV.y = m_posR.z + m_fDistance* cosf(m_fθ);					//カメラ位置Y
		m_posV.z = m_posR.y + m_fDistance* sinf(m_fθ) * cosf(m_fφ);	//カメラ位置Z
		break;

	default:
		break;
	}

	//状態の初期設定
	m_state = CAMERASTATE_NORMAL;

	return S_OK;
}

//=============================================================================
//カメラクラスの終了処理
//=============================================================================
void CCamera::Uninit(void)
{
	m_nCameraAll = 0;

	for (int nCount = 0; nCount < 2; nCount++)
	{
		if (m_pLockon[nCount] != NULL)
		{
			m_pLockon[nCount]->Uninit();
			m_pLockon[nCount] = NULL;
		}
	}
}

//=============================================================================
//カメラクラスの更新処理
//=============================================================================
void CCamera::Update(void)
{
	static bool bPlayerWin = false;	//プレイヤー勝利フラグ
	static int nWinPlayer = 0;		//Winプレイヤー番号

	//キーボードクラス情報の取得
	CInputKeyboard *pKeyInput = CManager::GetKeyboard();

	D3DXVECTOR3 PlayerPos[MAX_PLAYER];	//プレイヤー位置
	D3DXVECTOR3 PlayerRot[MAX_PLAYER];	//プレイヤー角度
	CPlayer::PLAYER_STATE PlayerState[MAX_PLAYER];
	bool PlayerWin[MAX_PLAYER];

	if (CGame::GetPlayer(m_nCameraNum) != NULL)
	{
		//プレイヤー1位置取得
		PlayerPos[m_nCameraNum] = CGame::GetPlayer(m_nCameraNum)->GetPos();
		//プレイヤー1角度取得
		PlayerRot[m_nCameraNum] = CGame::GetPlayer(m_nCameraNum)->GetRot();

		PlayerState[m_nCameraNum] = CGame::GetPlayer(m_nCameraNum)->GetState();

		PlayerWin[m_nCameraNum] = CGame::GetPlayer(m_nCameraNum)->GetWin();

		int nCameraSecond = 0;	//プレイヤー2カメラ保存用

								//カメラ番号から敵のカメラ番号の設定
		if (m_nCameraNum == 0)
		{
			nCameraSecond = 1;
		}
		else
		{
			nCameraSecond = 0;
		}

		//プレイヤー2角度取得
		PlayerPos[nCameraSecond] = CGame::GetPlayer(nCameraSecond)->GetPos();
		//プレイヤー2角度取得
		PlayerRot[nCameraSecond] = CGame::GetPlayer(nCameraSecond)->GetRot();

		PlayerState[nCameraSecond] = CGame::GetPlayer(nCameraSecond)->GetState();

		switch (m_state)
		{
		case CAMERASTATE_NONE:
			break;
		case CAMERASTATE_NORMAL://通常

			if (PlayerWin[m_nCameraNum] == true)
			{
				//勝敗決定時
				m_bTarget = false;					//ターゲット機能off
				m_fφ = 0;							//初期化
				m_fθ = 0;
				m_fDistance = 0;
				m_nWinPlayer = m_nCameraNum;
				m_state = CAMERASTATE_END;			//ステータス変更
			}

			if (PlayerState[m_nCameraNum] == CPlayer::PLAYER_STATE_EXPLOSION)
			{
				//勝敗決定時
				m_bTarget = false;					//ターゲット機能off
				m_fφ = 0;							//初期化
				m_fθ = 0;
				m_fDistance = 0;
				m_state = CAMERASTATE_END;			//ステータス変更
			}

			NomalUpdate(PlayerPos, PlayerRot);
			break;
		case CAMERASTATE_END://終了

			EndUpdate(PlayerPos, PlayerRot, m_nWinPlayer);

			if (PlayerState[m_nCameraNum] == CPlayer::PLAYER_STATE_NONE && PlayerWin[m_nCameraNum] == false)
			{
				m_fDistance = DISTANCE;
				m_fθ = CAMERA_DEFAULT_Fθ;
				m_fφ = PlayerRot[m_nCameraNum].y;
				m_state = CAMERASTATE_NORMAL;			//ステータス変更

				m_posVDest.x = PlayerPos[m_nCameraNum].x + m_fDistance * sinf(m_fθ) * sinf(m_fφ);	//カメラ位置X設定
				m_posVDest.y = PlayerPos[m_nCameraNum].y + PLAYER_HEIGHT + m_fDistance * cosf(m_fθ);				//カメラ位置Y設定
				m_posVDest.z = PlayerPos[m_nCameraNum].z + m_fDistance * sinf(m_fθ) * cosf(m_fφ);	//カメラ位置Z設定

				m_posRDest = D3DXVECTOR3(PlayerPos[m_nCameraNum].x, PlayerPos[m_nCameraNum].y + PLAYER_HEIGHT, PlayerPos[m_nCameraNum].z);	//注視点設定

				m_nWinPlayer = -1;
																																			//設定値の反映
				m_posV += (m_posVDest - m_posV);
				m_posR += (m_posRDest - m_posR);
			}
			break;
		case CAMERASTATE_MAX://STATEの最大数
			break;
		default:
			break;
		}
	}
}

//=============================================================================
//カメラクラスの試合後更新処理
//=============================================================================
void CCamera::EndUpdate(D3DXVECTOR3 PlayerPos[], D3DXVECTOR3 PlayerRot[], int nWinPlayer)
{
	if (nWinPlayer == m_nCameraNum)	//勝利カメラ処理
	{
		//初期化後なら
		if (m_fφ <= 0)
		{
			m_fφ = PlayerRot[m_nCameraNum].y + WIN_START_ROT_VERTICAL;												//プレイヤーの角度から横回転開始角度設定
			m_fθ = WIN_START_ROT_HORIZONTAL;																		//縦回転開始角度設定
			m_fDistance = WIN_START_DISTANCE;																		//距離の開始位置設定
			m_fDifference = WIN_END_ROT_VERTICAL - WIN_START_ROT_VERTICAL;				//プレイヤーの角度プラス開始終了角度の差
			m_fEndVertical = PlayerRot[m_nCameraNum].y + WIN_END_ROT_VERTICAL;										//終了角度
		}

		//変化量プラス処理
		m_fφ += m_fDifference / 100.0f * WIN_ROT_MOVE_SUBTRACTION;
		m_fθ += (WIN_END_ROT_HORIZONTAL - WIN_START_ROT_HORIZONTAL) / 100.0f * WIN_ROT_MOVE_SUBTRACTION;
		m_fDistance -= (WIN_START_DISTANCE - WIN_END_DISTANCE) / 100.0f * WIN_ROT_MOVE_SUBTRACTION;

		//下限上限設定
		if (m_fφ >= m_fEndVertical)
		{
			m_fφ = m_fEndVertical;			//限界値に戻す		
		}
		if (m_fθ >= WIN_END_ROT_HORIZONTAL)
		{
			m_fθ = WIN_END_ROT_HORIZONTAL;	//限界値に戻す
		}
		if (m_fDistance <= WIN_END_DISTANCE)
		{

			m_fDistance = WIN_END_DISTANCE;	//限界値に戻す
		}

		m_posVDest.x = PlayerPos[m_nCameraNum].x + m_fDistance * sinf(m_fθ) * sinf(m_fφ);	//カメラ位置X設定
		m_posVDest.y = PlayerPos[m_nCameraNum].y + m_fDistance * cosf(m_fθ);				//カメラ位置Y設定
		m_posVDest.z = PlayerPos[m_nCameraNum].z + m_fDistance * sinf(m_fθ) * cosf(m_fφ);	//カメラ位置Z設定

		m_posRDest = D3DXVECTOR3(PlayerPos[m_nCameraNum].x, PlayerPos[m_nCameraNum].y + PLAYER_HEIGHT, PlayerPos[m_nCameraNum].z);	//注視点設定

		//カメラPOSYの下限
		if (m_posVDest.y <= CAMERA_MIN_HIGHT)
		{
			m_posVDest.y = CAMERA_MIN_HIGHT;	//限界値に戻す
		}

		m_posV += (m_posVDest - m_posV); //カメラフロー
		m_posR += (m_posRDest - m_posR); //カメラフロー
	}
	else //敗北カメラ処理
	{
		//初期化後なら
		if (m_fDistance <= 0)
		{
			m_fφ = PlayerRot[m_nCameraNum].y + LOSE_START_ROT_VERTICAL;	//プレイヤーの角度から横回転開始角設定
			m_fθ = LOSE_START_ROT_HORIZONTAL;								//縦回転開始角設定
			m_fDistance = LOSE_START_DISTANCE;								//距離の設定
		}

		//変化量プラス処理
		m_fDistance -= (LOSE_START_DISTANCE - LOSE_END_DISTANCE) / 100.0f * LOSE_MOVE_SUBTRACTION;

		//下限上限設定
		if (m_fDistance >= LOSE_END_DISTANCE)
		{
			m_fDistance = LOSE_END_DISTANCE;	//限界値に戻す
		}

		m_posVDest.x = PlayerPos[m_nCameraNum].x + m_fDistance * sinf(m_fθ) * sinf(m_fφ);	//カメラ位置X設定
		m_posVDest.y = PlayerPos[m_nCameraNum].y + m_fDistance * cosf(m_fθ);				//カメラ位置Y設定
		m_posVDest.z = PlayerPos[m_nCameraNum].z + m_fDistance * sinf(m_fθ) * cosf(m_fφ);	//カメラ位置Z設定

		m_posRDest = D3DXVECTOR3(PlayerPos[m_nCameraNum].x, PlayerPos[m_nCameraNum].y + PLAYER_HEIGHT, PlayerPos[m_nCameraNum].z);	//注視点設定

																																	//カメラPOSYの下限
		if (m_posVDest.y <= CAMERA_MIN_HIGHT)
		{
			m_posVDest.y = CAMERA_MIN_HIGHT;	//限界値に戻す
		}

		//設定値の反映
		m_posV += (m_posVDest - m_posV); //カメラフロー
		m_posR += (m_posRDest - m_posR); //カメラフロー
	}
}

//=============================================================================
//カメラクラスの試合中更新処理
//=============================================================================
void CCamera::NomalUpdate(D3DXVECTOR3 PlayerPos[], D3DXVECTOR3 PlayerRot[])
{
	int nCameraSecond = 0;	//プレイヤー2カメラ保存用
	//カメラ番号から敵のカメラ番号の設定
	if (m_nCameraNum == 0)
	{
		nCameraSecond = 1;
	}
	else
	{
		nCameraSecond = 0;
	}

	//キーボードクラス情報の取得
	CInputKeyboard *pKeyInput = CManager::GetKeyboard();

	// ジョイパッドの取得
	DIJOYSTATE js = CInputJoypad::GetStick(m_nCameraNum);

	// ゲーム開始後
	if (CLife::GetReadey() == false)
	{		//入力処理
		if (CManager::GetJoypad()->GetJoystickTrigger(CInputJoypad::JOY_BUTTON_Y, m_nCameraNum) || pKeyInput->GetTrigger(DIK_TAB) && !m_bTarget&&CGame::GetPlayer(m_nCameraNum)->GetJump() == false)
		{
			if (CGame::GetPlayer(m_nCameraNum)->GetJump() == false)
			{
				//ターゲット機能OFF
				m_fθ = CAMERA_DEFAULT_Fθ;	//縦回転角固定
				m_bTarget = true;			//ターゲットON
			}
		}
	}

	//入力処理
	if (CManager::GetJoypad()->GetJoystickTrigger(CInputJoypad::JOY_BUTTON_R3, m_nCameraNum) || pKeyInput->GetTrigger(DIK_TAB) && m_bTarget)
	{
		//ターゲット機能ON
		m_fφ = PlayerRot[m_nCameraNum].y + PLAYE_ROT_VERTICAL_FRONT;	//横回転初期値
		m_bTarget = false;												//ターゲットOFF
	}

	//ターゲット機能ON
	if (m_bTarget == true)
	{
		//近すぎたらターゲット解除
		if (PlayerPos[nCameraSecond].x <= PlayerPos[m_nCameraNum].x + JUDGMENT_RANGE && PlayerPos[m_nCameraNum].x - JUDGMENT_RANGE <= PlayerPos[nCameraSecond].x  &&
			PlayerPos[nCameraSecond].z <= PlayerPos[m_nCameraNum].z + JUDGMENT_RANGE && PlayerPos[m_nCameraNum].z - JUDGMENT_RANGE <= PlayerPos[nCameraSecond].z)		{
			//ターゲットOFF
			m_bTarget = false;
		}
		//自分とターゲットの位置から角度取得
		m_fφ = atan2f(PlayerPos[m_nCameraNum].x - PlayerPos[nCameraSecond].x, PlayerPos[m_nCameraNum].z - PlayerPos[nCameraSecond].z);

		if (m_fφ < 0)
		{
			m_fφ = D3DXToRadian(360.0f + D3DXToDegree(m_fφ));
		}

		m_posVDest.x = m_posR.x + m_fDistance * sinf(m_fθ) * sinf(m_fφ) + PlayerPos[m_nCameraNum].x - PlayerPos[nCameraSecond].x;		//カメラ位置X設定
		m_posVDest.y = m_posR.y + m_fDistance * cosf(m_fθ) + PlayerPos[m_nCameraNum].y - PlayerPos[nCameraSecond].y;					//カメラ位置Y設定
		m_posVDest.z = m_posR.z + m_fDistance * sinf(m_fθ) * cosf(m_fφ) + PlayerPos[m_nCameraNum].z - PlayerPos[nCameraSecond].z;		//カメラ位置Z設定

		m_posRDest = D3DXVECTOR3(PlayerPos[nCameraSecond].x, PlayerPos[nCameraSecond].y + PLAYER_HEIGHT, PlayerPos[nCameraSecond].z);	//注視点設定

		m_posV += (m_posVDest - m_posV) *0.9; //カメラフロー
		m_posR += (m_posRDest - m_posR) *0.9; //カメラフロー

		if (m_nCameraNum == 0)
		{
			if (m_pLockon[0] == NULL)
			{
				m_pLockon[0] = CLockon::Create(D3DXVECTOR3(UI_LOCKON_POS_LEFT_X, UI_RESULT_POS_Y, 0.0f), D3DXVECTOR3(UI_LOCKON_SIZE_X, UI_LOCKON_SIZE_Y, 0.0f), CLockon::LOCKONTYPE_FAST_PLAYER);
			}
		}

		if (m_nCameraNum == 1)
		{
			if (m_pLockon[1] == NULL)
			{
				m_pLockon[1] = CLockon::Create(D3DXVECTOR3(UI_LOCKON_POS_RIGHT_X, UI_RESULT_POS_Y, 0.0f), D3DXVECTOR3(UI_LOCKON_SIZE_X, UI_LOCKON_SIZE_Y, 0.0f), CLockon::LOCKONTYPE_SECOND_PLAYER);
			}
		}
	}
	else //ターゲット機能OFF
	{

		//視点（カメラ座標）の左旋回
		if (pKeyInput->GetPress(DIK_LEFT) || js.lZ > STICK_SENSITIVITY)
		{
			m_fφ += STICK_INPUT_CONVERSION;
		}
		//視点（カメラ座標）の右旋回
		if (pKeyInput->GetPress(DIK_RIGHT) || js.lZ < -STICK_SENSITIVITY)
		{
			m_fφ -= STICK_INPUT_CONVERSION;
		}
		//視点（カメラ座標）の上旋回
		if (pKeyInput->GetPress(DIK_UP) || js.lRz > STICK_SENSITIVITY && m_fθ >= CAMERA_MIN_Fφ)
		{
			m_fθ -= STICK_INPUT_CONVERSION;
		}
		//視点（カメラ座標）の下旋回
		if (pKeyInput->GetPress(DIK_DOWN) || js.lRz < -STICK_SENSITIVITY && m_fθ <= CAMERA_MAX_Fφ)
		{
			m_fθ += STICK_INPUT_CONVERSION;
		}

		m_posVDest.x = PlayerPos[m_nCameraNum].x + m_fDistance * sinf(m_fθ) * sinf(m_fφ);	//カメラ位置X設定
		m_posVDest.y = PlayerPos[m_nCameraNum].y + PLAYER_HEIGHT + m_fDistance * cosf(m_fθ);				//カメラ位置Y設定
		m_posVDest.z = PlayerPos[m_nCameraNum].z + m_fDistance * sinf(m_fθ) * cosf(m_fφ);	//カメラ位置Z設定

		m_posRDest = D3DXVECTOR3(PlayerPos[m_nCameraNum].x, PlayerPos[m_nCameraNum].y + PLAYER_HEIGHT, PlayerPos[m_nCameraNum].z);	//注視点設定

		//カメラPOSYの下限
		if (m_posVDest.y <= CAMERA_MIN_HIGHT)
		{
			m_posVDest.y = CAMERA_MIN_HIGHT;	//限界値に戻す
		}

		//設定値の反映
		m_posV += (m_posVDest - m_posV)*0.1;
		m_posR += (m_posRDest - m_posR)*0.9;

		if (m_nCameraNum == 0)
		{
			if (m_pLockon[0] != NULL)
			{
				m_pLockon[0]->Uninit();
				m_pLockon[0] = NULL;
			}
		}

		if (m_nCameraNum == 1)
		{
			if (m_pLockon[1] != NULL)
			{
				m_pLockon[1]->Uninit();
				m_pLockon[1] = NULL;
			}
		}
	}
}

//=============================================================================
//カメラクラスのセット処理
//=============================================================================
void CCamera::SetCamera(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,
		&m_posR,
		&m_posU);

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	//プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT / 2,
		10.0f,
		100000.0f);

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION,
		&m_mtxProjection);
}

//=============================================================================
// ターゲットの設定
//=============================================================================
void CCamera::SetTarget(bool Target)
{
	m_bTarget = Target;
}

//=============================================================================
//カメラの位置取得処理
//=============================================================================
D3DXVECTOR3 CCamera::GetposV(void)
{
	return m_posV;
}

//=============================================================================
//カメラの角度取得処理
//=============================================================================
D3DXVECTOR3 CCamera::GetposR(void)
{
	return m_posR;
}

//=============================================================================
//カメラのビューマトリックス取得処理
//=============================================================================
D3DXMATRIX CCamera::GetMtxView(void)
{
	return m_mtxView;
}

//=============================================================================
//カメラのターゲットフラグ取得処理
//=============================================================================
bool CCamera::GetTargetBool(void)
{
	return m_bTarget;
}

//=============================================================================
//カメラの縦回転角取得処理
//=============================================================================
float CCamera::Getθ(void)
{
	return m_fθ;
}

//=============================================================================
//カメラの横回転角取得処理
//=============================================================================
float CCamera::Getφ(void)
{
	return m_fφ;
}