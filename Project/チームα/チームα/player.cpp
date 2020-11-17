//=============================================================================
//
// プレイヤークラス [player.cpp]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "player.h"
#include <stdio.h>
#include "manager.h"
#include "keyboard.h"
#include "renderer.h"
#include "input.h"
#include "bullet.h"
#include "joypad.h"
#include "charge.h"
#include "camera.h"
#include "life.h"
#include "game.h"
#include "beam.h"
#include "particle.h"
#include "bomb.h"
#include "2d_explosion.h"
#include "smoke.h"
#include "sand.h"
#include "bill.h"
#include "laser.h"
#include "locus.h"
#include "boost.h"
#include "sound.h"
#include "ui.h"
#include "time.h"
#include "missile.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define PLAYER_SPEED			(20.0f)				// プレイヤーの移動量
#define PLAYER_DUSH				(37.5f)				// プレイヤーのダッシュ
#define PLAYER_DUSH_INTER		(80)				// ダッシュができる長さ
#define DUSH_NONE_TIME			(50)				// ダッシュできない時間
#define PLAYER_JUMP				(10.0f)				// ジャンプの処理
#define GRAVITY_POWAR			(0.15f)				// 重力の強さ
#define PLAYER_FALL				(-13.0f)			// 急降下の処理
#define GROUND_RIMIT			(0.0f)				// 地面の制限
#define PLAYE_ROT_Y_FRONT		(D3DXToRadian(90.0f))	//プレイヤーの縦軸前
#define PLAYE_ROT_Y_LEFT		(D3DXToRadian(180.0f))	//プレイヤーの縦軸左
#define PLAYE_ROT_Y_RIGHT		(D3DXToRadian(0.0f))	//プレイヤーの縦軸右
#define PLAYE_ROT_Y_BUCK		(D3DXToRadian(-90.0f))	//プレイヤーの縦軸後
#define STICK_SENSITIVITY		(50.0f)					//スティック感度
#define STATE_DAMAGE_TIME		(100)				// ダメージ状態のカウント
#define STATE_EXPLOSION_TIME	(30)				// 爆発状態のカウント
#define STATE_EXPLOSION_END		(500)				// 爆発状態の終了フレーム
#define PLAYER_BOMB_INTER		(150)				// ボムの待機フレーム
#define PLAYER_MISSILE			(150)				// ミサイルの待機フレーム
#define PLAYER_LASER_INTER		(300)				// レーザーの待機フレーム
#define LBX_XFAILE_NAME "data/Text/motion_LBX.txt"	//LBXのファイルパス
#define GANDAMU_XFAILE_NAME "data/Text/motion_gandamu.txt"	//ガンダムのファイルパス
//=============================================================================
// グローバル変数宣言
//=============================================================================
MODELFILLE g_modelfile[MAX_MODEL_PARTS];	//モデルパーツ情報.

//=============================================================================
// static初期化
//=============================================================================
int CPlayer::m_nPlayerAll = 0;				// プレイヤーの総数

//=============================================================================
// クリエイト
//=============================================================================
CPlayer * CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// 初期化処理
	CPlayer *pPlayer = new CPlayer;

	// プレイヤーの番号を代入
	pPlayer->m_nPlayerNum = m_nPlayerAll++;

	// 初期化処理
	pPlayer->Init(pos, size);

	return pPlayer;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CPlayer::CPlayer(int nPriority) : CScene(nPriority)
{
	pScore = NULL;
	memset(m_pLife, 0, sizeof(m_pLife));
	m_pCharge = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_OldPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nDushFlame = 0;
	m_nDushInterCnt = 0;
	m_bJump = false;
	m_bDush = false;
	m_bDushInter = false;
	m_nPlayerNum = 0;						// プレイヤーの番号
	m_fAngle = 0.0f;
	m_state = PLAYER_STATE_NONE;
	m_nStateCounter = 0;
	m_bEnd = false;
	memset(m_pWinLose, 0, sizeof(m_pWinLose));
	m_bFall = false;	
	m_bWalk = false;
	memset(m_apModelAnime, 0, sizeof(m_apModelAnime));
	m_nNumKey = 0;
	m_apKeyInfo = NULL;
	m_nKey = 0;;
	m_nCountMotion = 0;
	m_nMotionInterval = 0;
	memset(&m_Motion, 0, sizeof(m_Motion));
	m_MotionState = MOTION_IDOL;
	memset(m_pBoost,0,sizeof(m_pBoost));
	m_bWinLose = false;
	m_bMotionPlaing = false;
	m_bEntered = false;
	m_bWinLose = false;
	m_bDraw = false;
	m_bArmor = false;
	m_nBombInter = 0;
	m_nMissileInter = 0;
	m_nLaserInter = 0;
	m_bAttack = false;
	m_bWin = false;
	m_pDraw = NULL;

}

//=============================================================================
// デストラクタ
//=============================================================================
CPlayer::~CPlayer()
{
	m_nPlayerAll = 0;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	pScore = NULL;
	memset(m_pLife, 0, sizeof(m_pLife));
	m_pCharge = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_OldPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nDushFlame = 0;
	m_nDushInterCnt = 0;
	m_bJump = false;
	m_bDush = false;
	m_bDushInter = false;
	m_fAngle = 0.0f;
	m_state = PLAYER_STATE_NONE;
	m_nStateCounter = 0;
	m_bEnd = false;
	m_bFall = false;
	memset(m_apModelAnime, 0, sizeof(m_apModelAnime));
	m_nNumKey = 0;
	m_apKeyInfo = NULL;
	m_nKey = 0;;
	m_nCountMotion = 0;
	memset(&m_Motion, 0, sizeof(m_Motion));
	m_MotionState = MOTION_IDOL;
	memset(m_pBoost, 0, sizeof(m_pBoost));
	m_bWinLose = false;
	m_bDraw = false;
	//CLife::SetReady(true);
	m_nBombInter = 0;							// ボムのインターバル
	m_nLaserInter = 0;;							// レーザーのインターバル
	m_bAttack = false;
	m_bWin = false;
	m_pDraw = NULL;

	for (int nCount = 0; nCount < MAX_PLAYER; nCount++)
	{
		m_pWinLose[nCount] = NULL;
	}

	//位置の設定
	m_pos = pos;

	switch (m_nPlayerNum)
	{
		//1Pだった場合
	case 0:
		//1Pのライフゲージ		
		if (m_pLife[0] == NULL)
		{
			//1P側に体力ゲージを生成
			m_pLife[0] = CLife::Create(D3DXVECTOR3(LIFE_POS_LEFT_X, LIFE_POS_UP_Y, 0.0f),

				D3DXVECTOR3(0.0f, LIFE_SIZE_PLAYER_Y, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 255),
				CLife::LIFETYPE_FAST_PLAYER);
		}

		//1Pのライフゲージ
		if (m_pLife[1] == NULL)
		{
			//2P側に体力ゲージを生成
			m_pLife[1] = CLife::Create(D3DXVECTOR3(LIFE_POS_RIGHT_X, LIFE_POS_DOWN_Y, 0.0f),
				D3DXVECTOR3(0.0f, LIFE_SIZE_ENEMY_Y, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 255),
				CLife::LIFETYPE_SECOND_PLAYER);
		}

		//１Ｐの弾のチャージゲージ
		if (m_pCharge == NULL)
		{
			//2P側に体力ゲージを生成
			m_pCharge = CCharge::Create(D3DXVECTOR3(CHARGE_POS_LEFT_X, CHARGE_POS_Y, 0.0f),
				D3DXVECTOR3(MAX_CHARGE, CHARGE_SIZE_Y, 0.0f), D3DCOLOR_RGBA(87, 210, 128, 255));
		}

		//ROTの初期値設定（敵の方向）
		m_rot = D3DXVECTOR3(0.0f, D3DXToRadian(180.0f), 0.0f);
		m_rotDest = D3DXVECTOR3(0.0f, D3DXToRadian(180.0f), 0.0f);
		break;

		//2Pだった場合
	case 1:
		//２Ｐのライフゲージ
		if (m_pLife[0] == NULL)
		{
			m_pLife[0] = CLife::Create(D3DXVECTOR3(LIFE_POS_RIGHT_X, LIFE_POS_UP_Y, 0.0f),
				D3DXVECTOR3(0.0f, LIFE_SIZE_PLAYER_Y, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 255),
				CLife::LIFETYPE_FAST_PLAYER);
		}

		//１Ｐのライフゲージ		
		if (m_pLife[1] == NULL)
		{
			m_pLife[1] = CLife::Create(D3DXVECTOR3(LIFE_POS_LEFT_X, LIFE_POS_DOWN_Y, 0.0f),
				D3DXVECTOR3(0.0f, LIFE_SIZE_ENEMY_Y, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 255),
				CLife::LIFETYPE_SECOND_PLAYER);
		}

		//２Ｐの弾のチャージゲージ
		if (m_pCharge == NULL)
		{
			m_pCharge = CCharge::Create(D3DXVECTOR3(CHARGE_POS_RIGHT_X, CHARGE_POS_Y, 0.0f),
				D3DXVECTOR3(MAX_CHARGE, CHARGE_SIZE_Y, 0.0f), D3DCOLOR_RGBA(87, 210, 128, 255));
		}

		//ROTの初期値設定（敵の方向）
		m_rotDest = D3DXVECTOR3(0.0f, D3DXToRadian(0.0f), 0.0f);

		break;

	default:
		break;
	}
	//ファイル読み込み
	if (FAILED(ReadFile()))
	{
		return E_FAIL;
	}

	//モデルパーツ数分繰り返す
	for (int nCntModel = 0; nCntModel < MAX_MODEL_PARTS; nCntModel++)
	{
		if (m_apModelAnime[nCntModel] == NULL)
		{
			//モデルの生成
			m_apModelAnime[nCntModel] = CModelAnime::Create(g_modelfile[nCntModel].xFileName,
				g_modelfile[nCntModel].offsetPos, g_modelfile[nCntModel].offsetRot);
		}
		//親子構造の設定
		if (m_apModelAnime[nCntModel] != NULL)
		{
			//親モデルの場合
			if (nCntModel == 0)
			{
				m_apModelAnime[nCntModel]->SetParent(NULL);
			}
			//子モデルの場合
			else
			{
				//自分の親情報を設定する
				m_apModelAnime[nCntModel]->SetParent(m_apModelAnime[g_modelfile[nCntModel].nParent]);
			}
		}
	}
	//待機モーションの再生
	SetMotion(MOTION_IDOL);

	//オブジェクトタイプの設定
	SetObjType(CScene::OBJTYPE_PLAYER);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CPlayer::Uninit(void)
{
	for (int nCntModelNum = 0; nCntModelNum < MAX_MODEL_PARTS; nCntModelNum++)
	{
		if (m_apModelAnime[nCntModelNum] != NULL)
		{
			//終了処理
			m_apModelAnime[nCntModelNum]->Uninit();

			//メモリの削除
			delete m_apModelAnime[nCntModelNum];

			//メモリのクリア
			m_apModelAnime[nCntModelNum] = NULL;
		}
	}

	//オブジェクトの破棄
	SetDeathFlag();
}

//=============================================================================
// 更新処理
//=============================================================================
void CPlayer::Update(void)
{
	// キーボード更新
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	// ボムとレーザーのフレーム加算
	m_nBombInter++;
	m_nLaserInter++;
	m_nMissileInter++;

	// 古い座標取得
	m_OldPos = m_pos;

	// プレイヤーの状態
	PlayerState();

	// ダッシュしていないとき
	if (m_bDush == false)
	{
		// 重力をかける
		m_move.y -= GRAVITY_POWAR;
		m_pos.y += m_move.y;		// 落下
	}
	// 地面の制限
	GroundLimit();

	// モーション状態
	MotionState();

	//アニメーションの更新処理
	UpdateMotion();

	// 終了フラグ
	switch (m_nPlayerNum)
	{
	case 0:
	{
		PLAYER_STATE state = CGame::GetPlayer(1)->GetState();

		if (state == PLAYER_STATE_EXPLOSION || state == PLAYER_STATE_DRAW)
		{
			if (m_bWin == false)
			{
				m_bWin = true;

				SetMotion(MOTION_WIN);
			}
			
			return;
		}
	}
	break;

	case 1:
	{
		PLAYER_STATE state = CGame::GetPlayer(0)->GetState();

		if (state == PLAYER_STATE_EXPLOSION || state == PLAYER_STATE_DRAW)
		{
			m_bWin = true;
			return;
		}
	}
	break;

	default:
		break;
	}

	if (m_pLife[0] != NULL)	
	{
		float fLife = m_pLife[0]->GetLife();

		char aData[1024];

		sprintf(aData, "%dの体力-> %f\n", m_nPlayerNum, fLife);

		OutputDebugString(aData);

		// 終了処理
		if (fLife <= 0.0f && m_pLife[0]->GetReadey() == false)
		{
			// 爆発状態
			if (m_state != PLAYER_STATE_EXPLOSION)
			{
				m_state = PLAYER_STATE_EXPLOSION;
				SetMotion(MOTION_LOSE);
			}
			

			//Uninit();
			return;
		}
	}

	// タイムリミット
	TimeLimit();

	// プレイヤーの制御
	PlayerControl();

	//位置へ移動量を加算
	m_pos += m_move;

	while (m_rotDest.y - m_rot.y > D3DXToRadian(180))
	{
		m_rotDest.y -= D3DXToRadian(360);
	}

	while (m_rotDest.y - m_rot.y < D3DXToRadian(-180))
	{
		m_rotDest.y += D3DXToRadian(360);
	}

	m_rot += (m_rotDest - m_rot)*0.1f;

	//ターゲットON
	if (CGame::GetCamera(m_nPlayerNum)->GetTargetBool())
	{
		//角度設定
		m_rotDest.y = CGame::GetCamera(m_nPlayerNum)->Getφ();
	}
	else //ターゲットOFF
	{
	}

	//範囲外に行かないようにする処理
	MapLimit();

	// 軌跡
	Locus();
}

//=============================================================================
// アニメーションの更新処理
//=============================================================================
void CPlayer::UpdateMotion(void)
{
	KEY *pKey[MAX_MODEL_PARTS];
	D3DXVECTOR3 diffPos, diffRot, startPos, startRot, setPos, setRot;

	//現在キーが最大キー数未満の場合
	if (m_nKey < m_Motion[m_MotionState].nNumKey)
	{
		for (int nCntModel = 0; nCntModel < MAX_MODEL_PARTS; nCntModel++)
		{
			m_apKeyInfo = &m_Motion[m_MotionState].aKeyInfo[m_nKey];

			pKey[nCntModel] = &m_apKeyInfo->aKey[nCntModel];
		}

		for (int nCntModel = 0; nCntModel < MAX_MODEL_PARTS; nCntModel++)
		{
			if (m_apModelAnime[nCntModel] != NULL)
			{
				D3DXVECTOR3 startPos = m_apModelAnime[nCntModel]->GetPosAnime();
				D3DXVECTOR3 startRot = m_apModelAnime[nCntModel]->GetRotAnime();

				//1フレーム当たりの更新値 = (終点位置-開始位置) / フレーム数
				diffPos.x = (pKey[nCntModel]->fPosX - startPos.x) / (float)m_Motion[m_MotionState].aKeyInfo[m_nKey].nFrame;
				diffPos.y = (pKey[nCntModel]->fPosY - startPos.y) / (float)m_Motion[m_MotionState].aKeyInfo[m_nKey].nFrame;
				diffPos.z = (pKey[nCntModel]->fPosZ - startPos.z) / (float)m_Motion[m_MotionState].aKeyInfo[m_nKey].nFrame;

				//1フレーム当たりの更新値 = (終点向き-開始向き) / フレーム数
				diffRot.x = (pKey[nCntModel]->fRotX - startRot.x) / (float)m_Motion[m_MotionState].aKeyInfo[m_nKey].nFrame;
				diffRot.y = (pKey[nCntModel]->fRotY - startRot.y) / (float)m_Motion[m_MotionState].aKeyInfo[m_nKey].nFrame;
				diffRot.z = (pKey[nCntModel]->fRotZ - startRot.z) / (float)m_Motion[m_MotionState].aKeyInfo[m_nKey].nFrame;

				setPos.x = diffPos.x * m_nCountMotion + startPos.x;
				setPos.y = diffPos.y * m_nCountMotion + startPos.y;
				setPos.z = diffPos.z * m_nCountMotion + startPos.z;

				setRot.x = diffRot.x * m_nCountMotion + startRot.x;
				setRot.y = diffRot.y * m_nCountMotion + startRot.y;
				setRot.z = diffRot.z * m_nCountMotion + startRot.z;

				D3DXVECTOR3 pos = m_apModelAnime[nCntModel]->GetPosAnime();
				D3DXVECTOR3 rot = m_apModelAnime[nCntModel]->GetRotAnime();

				//位置に更新用の位置を加算
				pos += setPos;

				//向きに更新用の向きを加算
				rot += setRot;

				//位置の設定
				m_apModelAnime[nCntModel]->SetPosAnime(setPos);

				//向きの設定
				m_apModelAnime[nCntModel]->SetRotAnime(setRot);
			}
		}

		//モーションカウンターの加算
		m_nCountMotion++;

		//現在キーの再生フレームに達したら
		if (m_nCountMotion == m_Motion[m_MotionState].aKeyInfo[m_nKey].nFrame)
		{
			//キーを１つ進める
			m_nKey++;
			m_nCountMotion = 0;
		}
	}
	else
	{
		//ループするなら
		if (m_Motion[m_MotionState].bLoop == true)
		{
			m_nKey = 0;
		}
		else
		{
			m_nMotionInterval++;

			if (m_nMotionInterval == 10)
			{
				m_bMotionPlaing = false;

				if (m_MotionState != MOTION_WIN && m_MotionState != MOTION_LOSE)
				{
					//着地・攻撃・左右ブースト・ビームモーションの時かつ入力がtrue・ジャンプがfalse
					if ((m_MotionState == MOTION_LANDING || m_MotionState == MOTION_ATTACK ||
						m_MotionState == MOTION_LEFTBOOST || m_MotionState == MOTION_RIGHTBOOST ||
						m_MotionState == MOTION_BEAM)
						&& m_bEntered == true && m_bJump == false)
					{
						SetMotion(MOTION_WALK);
					}
					else if (m_bJump == false)
					{
						SetMotion(MOTION_IDOL);
					}
				}
				else
				{
					return;
				}
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CPlayer::Draw(void)
{
	// 描画処理
	//デバイス情報の取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxScale;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);					// 裏面を（左回り）をカリング

	for (int nCntModelNum = 0; nCntModelNum < MAX_MODEL_PARTS; nCntModelNum++)
	{
		if (m_apModelAnime[nCntModelNum] != NULL)
		{

			//階層モデルクラスの描画処理
			m_apModelAnime[nCntModelNum]->Draw();
		}
	}

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);					// 裏面を（左回り）をカリング
}

//=============================================================================
// モーションセット処理
//=============================================================================
void CPlayer::SetMotion(MOTION_STATE motion)
{
	if (motion != MOTION_WIN && motion != MOTION_LOSE)
	{
		if (m_Motion[m_MotionState].bLoop == false &&
			m_bMotionPlaing == true && m_MotionState != MOTION_LANDING)
		{
			return;
		}
	}

	m_nKey = 0;
	m_nCountMotion = 0;
	m_nMotionInterval = 0;
	D3DXVECTOR3 pos, rot;
	
	m_MotionState = motion;
	m_bMotionPlaing = true;

	for (int nCntModel = 0; nCntModel < MAX_MODEL_PARTS; nCntModel++)
	{
		if (m_apModelAnime[nCntModel] != NULL)
		{
			//開始位置
			pos.x = m_Motion[m_MotionState].aKeyInfo[m_nKey].aKey[nCntModel].fPosX;
			pos.y = m_Motion[m_MotionState].aKeyInfo[m_nKey].aKey[nCntModel].fPosY;
			pos.z = m_Motion[m_MotionState].aKeyInfo[m_nKey].aKey[nCntModel].fPosZ;

			//開始向き
			rot.x = m_Motion[m_MotionState].aKeyInfo[m_nKey].aKey[nCntModel].fRotX;
			rot.y = m_Motion[m_MotionState].aKeyInfo[m_nKey].aKey[nCntModel].fRotY;
			rot.z = m_Motion[m_MotionState].aKeyInfo[m_nKey].aKey[nCntModel].fRotZ;

			//位置の設定
			m_apModelAnime[nCntModel]->SetPosAnime(pos);

			//向きの設定
			m_apModelAnime[nCntModel]->SetRotAnime(rot);
		}
	}

	switch (m_MotionState)
	{
	case MOTION_IDOL:

		// 無敵フラグの解除
		m_bArmor = false;
		m_bAttack = false;
		break;

	case MOTION_DAMAGE:

		// 無敵フラグ
		m_bArmor = true;
		break;
	case MOTION_ATTACK:

		// 攻撃フラグ
		m_bAttack = true;
		break;
	}
}

//=============================================================================
// プレイヤーの状態
//=============================================================================
void CPlayer::PlayerState(void)
{
	CSound *pSound = CManager::GetSound();

	switch (m_state)
	{
	case PLAYER_STATE_NORMAL:
		// 通常状態

		break;

	case PLAYER_STATE_DAMAGE:

		// ダメージを受けたら
		m_nStateCounter++;

		// 一定以上で
		if (m_MotionState == MOTION_DAMAGE && m_bMotionPlaing == false)
		{
			m_nStateCounter = 0;
			m_state = PLAYER_STATE_NORMAL;
		}
		break;

	case PLAYER_STATE_EXPLOSION:
		// 爆発状態
		m_nStateCounter++;

		
		// 勝ち負けロゴの出現
		if (m_bWinLose == false)
		{
			switch (m_nPlayerNum)
			{
			case 0:
			{
				SetPos(D3DXVECTOR3(0.0f, 0.0f, -1500.0f));
				CPlayer *pPlayer = CGame::GetPlayer(1);
				if (pPlayer != NULL)
				{

					pPlayer->SetPos(D3DXVECTOR3(PLAYER2_POS_X, 0.0f, 1500));
					pPlayer->SetWinToLose(true);

				}
				m_bWinLose = true;

				if (m_pWinLose[0] == NULL)
				{
					// 1Pが負けたとき
					m_pWinLose[0] = CUi::Create(D3DXVECTOR3(UI_RESULT_POS_RIGHT_X, UI_RESULT_POS_Y, 0.0f), D3DXVECTOR3(UI_RESULT_SIZE_X, UI_RESULT_SIZE_Y, 0.0f), CUi::UITYPE_WIN);
				}

				if (m_pWinLose[1] == NULL)
				{
					m_pWinLose[1] = CUi::Create(D3DXVECTOR3(UI_RESULT_POS_LEFT_X, UI_RESULT_POS_Y, 0.0f), D3DXVECTOR3(UI_RESULT_SIZE_X, UI_RESULT_SIZE_Y, 0.0f), CUi::UITYPE_LOSE);
				}

			}
				break;

			case 1:
			{
				m_bWinLose = true;

				SetPos(D3DXVECTOR3(PLAYER2_POS_X, 0.0f, 1500));

				CPlayer *pPlayer = CGame::GetPlayer(0);

				if (pPlayer != NULL)
				{
					pPlayer->SetPos(D3DXVECTOR3(0.0f, 0.0f, -1500.0f));
					pPlayer->SetWinToLose(true);
				}

				if (m_pWinLose[0] == NULL)
				{
					// 2Pが負けたとき
					m_pWinLose[0] = CUi::Create(D3DXVECTOR3(UI_RESULT_POS_LEFT_X, UI_RESULT_POS_Y, 0.0f), D3DXVECTOR3(UI_RESULT_SIZE_X, UI_RESULT_SIZE_Y, 0.0f), CUi::UITYPE_WIN);
				}

				if (m_pWinLose[1] == NULL)
				{
					m_pWinLose[1] = CUi::Create(D3DXVECTOR3(UI_RESULT_POS_RIGHT_X, UI_RESULT_POS_Y, 0.0f), D3DXVECTOR3(UI_RESULT_SIZE_X, UI_RESULT_SIZE_Y, 0.0f), CUi::UITYPE_LOSE);
				}


			}
				break;
			}
		}
		// 一定時間で
		if (m_nStateCounter % STATE_EXPLOSION_TIME == 0)
		{
			D3DXVECTOR3 TargetPos =
				D3DXVECTOR3(m_pos.x + rand() % 50 + rand() % 50 - rand() % 50 - rand() % 50,
					m_pos.y +150.0f + rand() % 100 + rand() % 100 - rand() % 50 - rand() % 50,
					m_pos.z + rand() % 50 + rand() % 50 - rand() % 50 - rand() % 50);

			// 爆発の生成
			C2dExplosion::Create(TargetPos,
				D3DXVECTOR3(EXPLOSION_SIZE_X_2D, EXPLOSION_SIZE_Y_2D, EXPLOSION_SIZE_Z_2D));
			//爆発音
			pSound->Play(CSound::SOUND_LABEL_SE_EXPLOSION_DEAH);

		}

		if (m_nStateCounter >= STATE_EXPLOSION_END)
		{
			//爆発音を止める
			pSound->Stop(CSound::SOUND_LABEL_SE_EXPLOSION_DEAH);
			// 終わりのフラグ
			m_bEnd = true;
		//	m_nStateCounter = PLAYER_STATE_NONE;

			switch (m_nPlayerNum)
			{
			case 0:
			{

				CPlayer *pPlayer = CGame::GetPlayer(1);

				for (int nCount = 0; nCount < MAX_PLAYER; nCount++)
				{
					// ライフの消去
					if (m_pLife[nCount] != NULL)
					{
						m_pLife[nCount]->Uninit();
						m_pLife[nCount] = NULL;
					}

					if (m_pWinLose[nCount] != NULL)
					{
						m_pWinLose[nCount]->Uninit();
						m_pWinLose[nCount] = NULL;
					}

					if (pPlayer->m_pLife[nCount] != NULL)
					{
						pPlayer->m_pLife[nCount]->Uninit();
						pPlayer->m_pLife[nCount] = NULL;
					}
					if (pPlayer->m_pWinLose[nCount] != NULL)
					{
						pPlayer->m_pWinLose[nCount]->Uninit();
						pPlayer->m_pWinLose[nCount] = NULL;
					}
				}
			}
			break;

			case 1:
			{
				CPlayer *pPlayer = CGame::GetPlayer(0);
				// ライフの消去
				if (m_pLife[1] != NULL)
				{
					m_pLife[1]->Uninit();
					m_pLife[1] = NULL;
				}

				if (pPlayer != NULL)
				{
					for (int nCount = 0; nCount < MAX_PLAYER; nCount++)
					{
						// ライフの消去
						if (m_pLife[nCount] != NULL)
						{
							m_pLife[nCount]->Uninit();
							m_pLife[nCount] = NULL;
						}
						if (m_pWinLose[nCount] != NULL)
						{

							m_pWinLose[nCount]->Uninit();
							m_pWinLose[nCount] = NULL;
						}

						if (pPlayer->m_pLife[nCount] != NULL)
						{
							pPlayer->m_pLife[nCount]->Uninit();
							pPlayer->m_pLife[nCount] = NULL;
						}

						if (pPlayer->m_pWinLose[nCount] != NULL)
						{
							pPlayer->m_pWinLose[nCount]->Uninit();
							pPlayer->m_pWinLose[nCount] = NULL;
						}
					}
				}
			}
			}
			break;
	case PLAYER_STATE_DRAW:
		
		if (m_bDraw == false)
		{
			SetMotion(MOTION_WIN);
			m_bDraw = true;
		}
		switch (m_nPlayerNum)
		{
		case 0:

			if (m_pDraw == NULL)
			{
				m_pDraw = CUi::Create(D3DXVECTOR3(UI_RESULT_POS_LEFT_X, UI_RESULT_POS_Y, 0.0f), D3DXVECTOR3(UI_RESULT_SIZE_X, UI_RESULT_SIZE_Y, 0.0f), CUi::UITYPE_DRAW);
			}
			break;
		case 1:

			if (m_pDraw == NULL)
			{
				m_pDraw = CUi::Create(D3DXVECTOR3(UI_RESULT_POS_RIGHT_X, UI_RESULT_POS_Y, 0.0f), D3DXVECTOR3(UI_RESULT_SIZE_X, UI_RESULT_SIZE_Y, 0.0f), CUi::UITYPE_DRAW);
			}
			break;

		}

		// 引き分けの判定
		m_nStateCounter++;

		if (m_nStateCounter >= STATE_EXPLOSION_END)
		{
			// 終わりのフラグ
			m_bEnd = true;

			switch (m_nPlayerNum)
			{
			case 0:
			{
				CPlayer *pPlayer = CGame::GetPlayer(1);

				if (pPlayer != NULL)
				{
					for (int nCount = 0; nCount < MAX_PLAYER; nCount++)
					{
						// ライフの消去
						if (m_pLife[nCount] != NULL)
						{
							m_pLife[nCount]->Uninit();
							m_pLife[nCount] = NULL;
						}
						if (pPlayer->m_pLife[nCount] != NULL)
						{
							pPlayer->m_pLife[nCount]->Uninit();
							pPlayer->m_pLife[nCount] = NULL;
						}
					}

					if (m_pDraw != NULL)
					{
						m_pDraw->Uninit();
						m_pDraw = NULL;
					}

					if (pPlayer->m_pDraw != NULL)
					{
						pPlayer->m_pDraw->Uninit();
						pPlayer->m_pDraw = NULL;
					}
				}
			}
			break;

			case 1:
			{
				CPlayer *pPlayer = CGame::GetPlayer(0);

				if (pPlayer != NULL)
				{
					for (int nCount = 0; nCount < MAX_PLAYER; nCount++)
					{
						// ライフの消去
						if (m_pLife[nCount] != NULL)
						{
							m_pLife[nCount]->Uninit();
							m_pLife[nCount] = NULL;
						}
						if (pPlayer->m_pLife[nCount] != NULL)
						{
							pPlayer->m_pLife[nCount]->Uninit();
							pPlayer->m_pLife[nCount] = NULL;
						}
					}

					if (m_pDraw != NULL)
					{
						m_pDraw->Uninit();
						m_pDraw = NULL;
					}

					if (pPlayer->m_pDraw != NULL)
					{
						pPlayer->m_pDraw->Uninit();
						pPlayer->m_pDraw = NULL;
					}

				}

			}
			break;

			default:
				break;
			}
		}
		}
	}
}

//=============================================================================
// モーション状態
//=============================================================================
void CPlayer::MotionState(void)
{
	switch (m_MotionState)
	{
	case MOTION_IDOL:
		//	m_MotionState = MOTION_ATTACK;
		break;
	case MOTION_ATTACK:
		// 攻撃モーション
		if (m_nKey >= 1 && m_nKey <= 3)
		{
			if (m_bArmor == false)
			{
				// 武器の当たり判定
				WeaponCollision();
			}
		}
		break;
	case MOTION_DAMAGE:
		
		break;
	}
}

//=============================================================================
// 武器の当たり判定
//=============================================================================
void CPlayer::WeaponCollision(void)
{
	CPlayer *pPlayer = NULL;

	// プレイヤー情報の取得
	switch (m_nPlayerNum)
	{
	case 0:
		pPlayer = CGame::GetPlayer(1);
		break;
	case 1:
		pPlayer = CGame::GetPlayer(0);
		break;
	}

	if (pPlayer != NULL)
	{
		//位置の取得
		D3DXVECTOR3 targetPos = pPlayer->GetPos();

		if (pPlayer->GetArmor() == false)
		{
			D3DXVECTOR3 Top = D3DXVECTOR3(
				m_apModelAnime[20]->GetMtxWorld()._41,
				m_apModelAnime[20]->GetMtxWorld()._42,
				m_apModelAnime[20]->GetMtxWorld()._43);

			// 当たり判定
			if (targetPos.x - PLAYER_COLLISION_X / 2 <= Top.x + WEAPON_COLLISION_X / 2 &&
				targetPos.x + PLAYER_COLLISION_X / 2 >= Top.x - WEAPON_COLLISION_X / 2 &&
				targetPos.y - 0.0f <= Top.y + WEAPON_COLLISION_Y / 2 &&
				targetPos.y + PLAYER_COLLISION_Y >= Top.y - WEAPON_COLLISION_Y / 2 &&
				targetPos.z - PLAYER_COLLISION_Z / 2 <= Top.z + WEAPON_COLLISION_Z / 2 &&
				targetPos.z + PLAYER_COLLISION_Z / 2 >= Top.z - WEAPON_COLLISION_Z / 2)
			{
				// 爆発の生成
				C2dExplosion::Create(D3DXVECTOR3(targetPos.x, 250.0f, targetPos.z),
					D3DXVECTOR3(EXPLOSION_SIZE_X_2D, EXPLOSION_SIZE_Y_2D, EXPLOSION_SIZE_Z_2D));

				// プレイヤー情報の取得
				switch (m_nPlayerNum)
				{
				case 0:

					CGame::GetCamera(1)->SetTarget(false);

					for (int nCount = 0; nCount < MAX_PLAYER; nCount++)
					{
						CGame::GetPlayer(1)->GetLife(nCount)->Decrease(100, m_nPlayerNum, true);

					}

					CGame::GetPlayer(1)->SetMotion(MOTION_DAMAGE);

					break;
				case 1:
					CGame::GetCamera(0)->SetTarget(false);

					for (int nCount = 0; nCount < MAX_PLAYER; nCount++)
					{
						CGame::GetPlayer(0)->GetLife(nCount)->Decrease(100, m_nPlayerNum, true);

					}

					CGame::GetPlayer(0)->SetMotion(MOTION_DAMAGE);

					break;
				}
			}
		}

	}
}

//=============================================================================
// タイム制限
//=============================================================================
void CPlayer::TimeLimit(void)
{
	// タイム取得
	CTime *pTime = CGame::GetTime();

	if (pTime != NULL)
	{
		// 制限時間を
		if (pTime->GetTime() <= 0)
		{
			pTime->SetTime(0);

			if (m_nPlayerNum == 0)
			{
				float fSizeX[MAX_PLAYER] = { 0.0f, 0.0f };

				fSizeX[0] = m_pLife[0]->GetSize().x;
				fSizeX[1] = CGame::GetPlayer(1)->GetLife(0)->GetSize().x;

				// 勝敗判定
				if (fSizeX[0] > fSizeX[1])
				{
					// 1Pが買ったら
					CPlayer *pPlayer = CGame::GetPlayer(1);

					if (pPlayer->GetState() != PLAYER_STATE_EXPLOSION)
					{
						pPlayer->SetState(PLAYER_STATE_EXPLOSION);

						//相手は負けモーションを再生
						pPlayer->SetMotion(MOTION_LOSE);

						//自分は勝利モーションを再生
						SetMotion(MOTION_WIN);
					}
					
					return;
				}
				else if (fSizeX[0] < fSizeX[1])
				{
					// 2Pが買ったら
					CPlayer *pPlayer = CGame::GetPlayer(0);

					if (pPlayer->GetState() != PLAYER_STATE_EXPLOSION)
					{
						pPlayer->SetState(PLAYER_STATE_EXPLOSION);

						//相手は負けモーションを再生
						pPlayer->SetMotion(MOTION_LOSE);

						//自分は勝利モーションを再生
						SetMotion(MOTION_WIN);
					}
					return;
				}
				else
				{
					for (int nCount = 0; nCount < MAX_PLAYER; nCount++)
					{
						// 2Pが買ったら
						CPlayer *pPlayer = CGame::GetPlayer(nCount);
						pPlayer->SetState(PLAYER_STATE_DRAW);
					}
				}
			}
		}
	}
}


//=============================================================================
// プレイヤーの制御
//=============================================================================
void CPlayer::PlayerControl()
{
	if (m_MotionState != MOTION_DAMAGE)
	{
		if (m_pLife[0]->GetReadey() == false)
		{
			// ダッシュしていないとき
			if (m_bDush == false)
			{
				// プレイヤーの移動処理
				Walk();
			}

			// ジャンプの処理
			Jump();

			// 急降下の処理
			Fall();

			// 回避の処理
			Dush();

			// ビームの処理
			beam();

			switch (m_nPlayerNum)
			{
			case 0:
				// ボムの処理
				bomb();
				break;

			case 1:
				// ミサイルの処理
				Missile();
				break;
			}

			if (CGame::GetCamera(m_nPlayerNum)->GetTargetBool() == true)
			{
				// レーザーの処理
				Laser();
			}
		}
	}
}

//=============================================================================
// プレイヤー移動処理
//=============================================================================
void CPlayer::Walk(void)
{
	// キーボード更新
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	// ジョイパッドの取得
	DIJOYSTATE js = CInputJoypad::GetStick(m_nPlayerNum);

	CSound *pSound = CManager::GetSound();

	if (m_bAttack == false)
	{
		if (pKeyboard->GetTrigger(DIK_I) ||
			CManager::GetJoypad()->GetJoystickTrigger(CInputJoypad::JOY_BUTTON_L_TRIGGER, m_nPlayerNum))
		{
			//攻撃モーションを再生
			SetMotion(MOTION_ATTACK);
			pSound->Play(CSound::SOUND_LABEL_SE_SLASH);
		}
	}
	//入力が存在する
	if ((js.lX != 0.0f || js.lY != 0.0f )&& m_bAttack == false)
	{
		//入力ありにする
		m_bEntered = true;

		//ダメージを受けていないときのみ移動する
		if (m_MotionState != MOTION_DAMAGE)
		{
			//ジャンプしていないとき
			if (m_bJump == false && m_bWalk == false)
			{
				m_bWalk = true;
				//歩行モーションの再生
				SetMotion(MOTION_WALK);
			}
			//スティックXの入力が感度超えている
			if (js.lX < -STICK_SENSITIVITY)
			{
				m_rotDest.y = m_fAngle + D3DXToRadian(270.0f);

				m_pos.x += cosf(m_fAngle)* PLAYER_SPEED;
				m_pos.z -= sinf(m_fAngle)* PLAYER_SPEED;
			}

			else if (js.lX > STICK_SENSITIVITY)
			{
				m_rotDest.y = m_fAngle + D3DXToRadian(90.0f);

				m_pos.x -= cosf(m_fAngle)* PLAYER_SPEED;
				m_pos.z += sinf(m_fAngle)* PLAYER_SPEED;
			}
			else
			{
			}
			//スティックYの入力が感度を超えている
			if (js.lY < -STICK_SENSITIVITY)
			{
				m_rotDest.y = m_fAngle;

				m_pos.x -= sinf(m_fAngle)* PLAYER_SPEED;
				m_pos.z -= cosf(m_fAngle)* PLAYER_SPEED;

				if (js.lX < -STICK_SENSITIVITY)
				{

					m_rotDest.y = m_fAngle + D3DXToRadian(315.0f);
				}
				else if (js.lX > STICK_SENSITIVITY)
				{

					m_rotDest.y = m_fAngle + D3DXToRadian(45.0f);
				}
			}

			else if (js.lY > STICK_SENSITIVITY)
			{
				m_rotDest.y = m_fAngle + D3DXToRadian(180.0f);
				m_pos.x += sinf(m_fAngle)* PLAYER_SPEED;
				m_pos.z += cosf(m_fAngle)* PLAYER_SPEED;

				if (js.lX < -STICK_SENSITIVITY)
				{
					m_rotDest.y = m_fAngle + D3DXToRadian(225.0f);
				}

				else if (js.lX > STICK_SENSITIVITY)
				{

					m_rotDest.y = m_fAngle + D3DXToRadian(135.0f);
				}
			}

		}
		// Wキーを押したとき
		if (pKeyboard->GetPress(DIK_W))
		{
			m_rotDest.y = D3DXToDegree(m_fAngle);
			m_pos.z += cosf(0)*PLAYER_SPEED;
		}

		// Sキーを押したとき
		if (pKeyboard->GetPress(DIK_S))
		{

			m_rotDest.y = sin(m_fAngle);
			m_pos.z += cosf(D3DX_PI)*PLAYER_SPEED;
		}
		// Aキーを押したとき
		if (pKeyboard->GetPress(DIK_A))
		{
			m_pos.x += sinf(-D3DX_PI / 2)*PLAYER_SPEED;
		}
		// Dキーを押したとき
		if (pKeyboard->GetPress(DIK_D))
		{
			m_pos.x += sinf(D3DX_PI / 2)*PLAYER_SPEED;
		}
	}
	else
	{
		m_bEntered = false;
		if (m_bWalk == true)
		{
			//待機モーションを再生
			SetMotion(MOTION_IDOL);
			m_bWalk = false;
		}
	}
}
//=============================================================================
// ジャンプ処理
//=============================================================================
void CPlayer::Jump(void)
{
	CSound *pSound = CManager::GetSound();
	// キーボード更新
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	// SPACEキーを押したとき・コントローラのYを押したとき
		if (CManager::GetJoypad()->GetJoystickTrigger(CInputJoypad::JOY_BUTTON_Y, m_nPlayerNum) && m_bJump == false	
			|| pKeyboard->GetTrigger(DIK_SPACE) && m_bJump == false)
	{
		// ジャンプの処理
		m_move.y = 0.0f;
		m_move.y = PLAYER_JUMP;
		m_bJump = true;
		m_bWalk = false;

		//ジャンプモーションの再生
		SetMotion(MOTION_JUMP);
		pSound->Play(CSound::SOUND_LABEL_SE_JUMP);
	}
}

//=============================================================================
// 地面の制限
//=============================================================================
void CPlayer::GroundLimit(void)
{
	CSound *pSound = CManager::GetSound();

	// 着地の処理
	if (m_pos.y <= GROUND_RIMIT)
	{
		if (m_bJump == true)
		{
			SetMotion(MOTION_LANDING);
		}
		m_move.y = 0.0f;
		m_pos.y = GROUND_RIMIT;
		m_bJump = false;
		
		if (m_bFall == true)
		{
			// 急降下を使用していない状態にする
			m_bFall = false;

			// 煙の生成
			CSmoke::Create(D3DXVECTOR3(m_pos.x, 0.0f, m_pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 
				D3DXVECTOR3(SMOKE_SIZE_X, SMOKE_SIZE_Y, SMOKE_SIZE_Z));

			//着地音
			pSound->Play(CSound::SOUND_LABEL_SE_SAND);

		}
	}
}

//=============================================================================
// 急降下の処理
//=============================================================================
void CPlayer::Fall(void)
{
	// キーボード更新
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	// SPACEキーを押したとき
	if (pKeyboard->GetTrigger(DIK_B) && m_bJump == true ||
		CManager::GetJoypad()->GetJoystickTrigger(CInputJoypad::JOY_BUTTON_A, m_nPlayerNum) && m_bJump == true)
	{
		// ジャンプの処理
		m_move.y = 0.0f;
		m_move.y = PLAYER_FALL;
		m_bFall = true;
	}
}

//=============================================================================
// ダッシュの処理
//=============================================================================
void CPlayer::Dush(void)
{
	// キーボード更新
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();
	CSound *pSound = CManager::GetSound();

	// ダッシュが使えるとき
	if (m_bDushInter == false)
	{
		//カメラ角度取得
		m_fAngle = CGame::GetCamera(m_nPlayerNum)->Getφ();

		// Xボタンの時
		if (CManager::GetJoypad()->GetJoystickTrigger(CInputJoypad::JOY_BUTTON_X, m_nPlayerNum))
		{
			// ジョイパッドの取得
			DIJOYSTATE js = CInputJoypad::GetStick(m_nPlayerNum);

			//入力が存在する
			if (js.lX != 0.0f || js.lY != 0.0f)
			{
				//移動量初期化
				m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

				//ダッシュ時移動量設定
				m_move.x += cosf(m_fAngle)* PLAYER_DUSH;
				m_move.z -= sinf(m_fAngle)* PLAYER_DUSH;

				m_bDush = true;
				m_bWalk = false;
				//ターボ音
				pSound->Play(CSound::SOUND_LABEL_SE_TURBO);

				// 地上にいたら
				if (m_bJump == false)
				{
					// 砂の生成
					CSand::Create(D3DXVECTOR3(m_pos.x, 0.0f, m_pos.z),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(m_rot.x + 0.0f, m_rot.y, m_rot.z+0.0f),
						D3DXVECTOR3(SAND_SIZE_X, SAND_SIZE_Y, SAND_SIZE_Z));
				}
				D3DXVECTOR3 TargetPos = D3DXVECTOR3(m_pos.x - sinf(m_rot.y) * 70.0f, m_pos.y - 50.0f, m_pos.z - cosf(m_rot.y) * 70.0f);

				for (int nCount = 0; nCount <= MAX_BOOST; nCount++)
				{
					if (m_pBoost[nCount] == NULL)
					{
						// ブースト処理
						m_pBoost[nCount] = CBoost::Create(TargetPos + m_move,
							D3DXVECTOR3(m_rot.x + D3DXToRadian(270.0f), m_rot.y + 0.0f, m_rot.z + 0.0f), D3DXVECTOR3(BOOST_SIZE_X, BOOST_SIZE_Y, BOOST_SIZE_Z), m_nPlayerNum);
					}
				}

				//左ブーストモーションの再生
				SetMotion(MOTION_LEFTBOOST);


				// 地上にいたら
				if (m_bJump == false)
				{
					// 砂の生成
					CSand::Create(D3DXVECTOR3(m_pos.x, 0.0f, m_pos.z),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(m_rot.x + 0.0f, m_rot.y, m_rot.z + 0.0f),
						D3DXVECTOR3(SAND_SIZE_X, SAND_SIZE_Y, SAND_SIZE_Z));
				}
			}
		}

		// Bボタンの時
		if (CManager::GetJoypad()->GetJoystickTrigger(CInputJoypad::JOY_BUTTON_B, m_nPlayerNum))
		{
			// ジョイパッドの取得
			DIJOYSTATE js = CInputJoypad::GetStick(m_nPlayerNum);

			if (js.lX != 0.0f || js.lY != 0)
			{
				//移動量初期化
				m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				//ダッシュ時移動量設定
				m_move.x -= cosf(m_fAngle)* PLAYER_DUSH;
				m_move.z += sinf(m_fAngle)* PLAYER_DUSH;

				m_bDush = true;
				//右ブーストモーションの再生
				SetMotion(MOTION_RIGHTBOOST);
				//ターボ音
				pSound->Play(CSound::SOUND_LABEL_SE_TURBO);				

				// 地上にいたら
				if (m_bJump == false)
				{
					// 砂の生成
					CSand::Create(D3DXVECTOR3(m_pos.x, 0.0f, m_pos.z),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(m_rot.x , m_rot.y + 3.14f, m_rot.z + 0.0f),
						D3DXVECTOR3(SAND_SIZE_X, SAND_SIZE_Y, SAND_SIZE_Z));
				}
			}
		}

		// Wキーを押したとき
		if (pKeyboard->GetPress(DIK_W) && pKeyboard->GetTrigger(DIK_RSHIFT))
		{
			m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_move.z += cosf(0)*PLAYER_DUSH;
			m_bDush = true;
		}
		// Sキーを押したとき
		if (pKeyboard->GetPress(DIK_S) && pKeyboard->GetTrigger(DIK_RSHIFT))
		{
			m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_move.z += cosf(D3DX_PI)*PLAYER_DUSH;
			m_bDush = true;
		}
		// Aキーを押したとき
		if (pKeyboard->GetPress(DIK_A) && pKeyboard->GetTrigger(DIK_RSHIFT))
		{
			m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_move.x -= sinf(D3DX_PI / 2)*PLAYER_DUSH;
			m_bDush = true;

			D3DXVECTOR3 TargetPos = D3DXVECTOR3(m_pos.x - sinf(m_rot.y) * 70.0f, m_pos.y - 50.0f, m_pos.z - cosf(m_rot.y) * 70.0f);

			for (int nCount = 0; nCount < MAX_BOOST; nCount++)
			{
				if (m_pBoost[nCount] == NULL)
				{
					// ブースト処理
					m_pBoost[nCount] = CBoost::Create(TargetPos + m_move,
						D3DXVECTOR3(m_rot.x + D3DXToRadian(270.0f), m_rot.y + 0.0f, m_rot.z + 0.0f), D3DXVECTOR3(BOOST_SIZE_X, BOOST_SIZE_Y, BOOST_SIZE_Z), m_nPlayerNum);
				}
			}
		}
		// Dキーを押したとき
		if (pKeyboard->GetPress(DIK_D) && pKeyboard->GetTrigger(DIK_RSHIFT))
		{
			m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_move.x += sinf(D3DX_PI / 2)*PLAYER_DUSH;
			m_bDush = true;
		}
	}
	else
	{
		m_move.x = 0.0f;
		m_move.z = 0.0f;

		m_nDushInterCnt++;
	}

	// ダッシュしているとき
	if (m_bDush == true)
	{
		// ダッシュが終わるまでをカウント
		m_nDushFlame++;

		//int nNum = rand() % 3;

		//if (nNum % 3 == 0)
		//{
		//	// 火花の生成
		//	for (int nCount = 0; nCount < 2; nCount++)
		//	{
		//		CParticle::Create(D3DXVECTOR3(m_pos.x, m_pos.y + 25.0f, m_pos.z), D3DXVECTOR3(SPARK_SIZE_X, SPARK_SIZE_Y, 0.0f),
		//			D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f), SPARK_ANGLE, SPARK_BASE_ANGLE, SPARK_DISTANCE, SPARK_LIFE, SPARK_SPPED);
		//	}
		//}
	}

	// ダッシュが終わるフレーム
	if (m_nDushFlame >= PLAYER_DUSH_INTER)
	{
		m_nDushFlame = 0;
		m_bDush = false;
		m_bDushInter = true;

		for (int nCount = 0; nCount < MAX_BOOST; nCount++)
		{
			if (m_pBoost[nCount] != NULL)
			{
				m_pBoost[nCount]->Uninit();
				m_pBoost[nCount] = NULL;
			}
		}
	}

	// ダッシュができるようになるフレーム
	if (m_nDushInterCnt >= DUSH_NONE_TIME)
	{
		m_nDushInterCnt = 0;
		m_bDushInter = false;
	}
}

//=============================================================================
// ビームの処理
//=============================================================================
void CPlayer::beam(void)
{
	// キーボード更新
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();
	CSound *pSound = CManager::GetSound();

	// Lキーを押したとき・コントローラのR1を押したとき
	if (CManager::GetJoypad()->GetJoystickTrigger(CInputJoypad::JOY_BUTTON_R_TRIGGER, m_nPlayerNum) && m_bJump == false || pKeyboard->GetTrigger(DIK_L) && m_bJump == false )
	{
		switch (m_nPlayerNum)
		{
		case 0:

			if (CCharge::GetCharge(0) > PLAYER_LASER)
			{

				//バレットの生成
				CBeam::Create(D3DXVECTOR3(m_pos.x, m_pos.y + 200.0f, m_pos.z), D3DXVECTOR3(0.0f, 100.0f, -BEAM_SPEED), D3DXVECTOR3(BEAM_SIZE_X, BEAM_SIZE_Y, BEAM_SIZE_Z), CBullet2::BULLET2_USER_PL1);
				//弾うったらゲージを減らす
				m_pCharge->Reduce(PLAYER_LASER, true, 0);
				pSound->Play(CSound::SOUND_LABEL_SE_BULLET);
			}
			break;

		case 1:
			
			if (CCharge::GetCharge(1) > PLAYER_LASER)
			{
				//バレットの生成
				CBeam::Create(D3DXVECTOR3(m_pos.x, m_pos.y + 200.0f, m_pos.z), D3DXVECTOR3(0.0f, 100.0f, BEAM_SPEED), D3DXVECTOR3(BEAM_SIZE_X, BEAM_SIZE_Y, BEAM_SIZE_Z), CBullet2::BULLET2_USER_PL2);
				//弾うったらゲージを減らす
				m_pCharge->Reduce(PLAYER_LASER, true, 1);
				pSound->Play(CSound::SOUND_LABEL_SE_BULLET);
			}
			break;
		}
	}
}

//=============================================================================
// 軌跡の処理
//=============================================================================
void CPlayer::Locus(void)
{
	// 古い座標取得
	D3DXVECTOR3 OldOrigin = D3DXVECTOR3(
		m_apModelAnime[19]->GetOldMtxWorld()._41,
		m_apModelAnime[19]->GetOldMtxWorld()._42,
		m_apModelAnime[19]->GetOldMtxWorld()._43);

	D3DXVECTOR3 OldTop = D3DXVECTOR3(
		m_apModelAnime[20]->GetOldMtxWorld()._41,
		m_apModelAnime[20]->GetOldMtxWorld()._42,
		m_apModelAnime[20]->GetOldMtxWorld()._43);

	D3DXVECTOR3 Origin = D3DXVECTOR3(
		m_apModelAnime[19]->GetMtxWorld()._41,
		m_apModelAnime[19]->GetMtxWorld()._42,
		m_apModelAnime[19]->GetMtxWorld()._43);

	D3DXVECTOR3 Top = D3DXVECTOR3(
		m_apModelAnime[20]->GetMtxWorld()._41,
		m_apModelAnime[20]->GetMtxWorld()._42,
		m_apModelAnime[20]->GetMtxWorld()._43);

	// 軌跡の生成
	CLocus::Create(Origin, Top, OldOrigin, OldTop,
		D3DXVECTOR3(m_rot.x, m_rot.y, m_rot.z), D3DXVECTOR3(LOCUS_SIZE_X, LOCUS_SIZE_Y, LOCUS_SIZE_Z),
		LOCUS_LIFE);

}

//=============================================================================
// ミサイル
//=============================================================================
void CPlayer::Missile(void)
{
	if (m_nMissileInter >= PLAYER_BOMB_INTER)
	{
		// キーボード更新
		CInputKeyboard *pKeyboard = CManager::GetKeyboard();
		CSound *pSound = CManager::GetSound();

		// Lキーを押したとき・コントローラのR1を押したとき
		if (CManager::GetJoypad()->GetJoystickTrigger(CInputJoypad::JOY_BUTTON_R2_TRIGGER, m_nPlayerNum) && m_bJump == false
			|| pKeyboard->GetTrigger(DIK_V) && m_bJump == false)
		{
			m_nMissileInter = 0;
			switch (m_nPlayerNum)
			{
			case 0:
				if (CCharge::GetCharge(0) > PLAYER_BOMB)
				{
					//バレットの生成
					CMissile::Create(D3DXVECTOR3(m_pos.x, m_pos.y + 50.0f, m_pos.z),
						D3DXVECTOR3(MISSILE_SIZE_X, MISSILE_SIZE_Y, MISSILE_SIZE_Z),
						CMissile::MISSILE_USER_PL1, MISSILE_SPPED);

					//発射音
					pSound->Play(CSound::SOUND_LABEL_SE_BULLET2);
					//弾うったらゲージを減らす

					if (m_pCharge != NULL)
					{
						m_pCharge->Reduce(PLAYER_BOMB, true, 0);
					}
				}
				break;

			case 1:

				if (CCharge::GetCharge(1) > PLAYER_BOMB)
				{
					//バレットの生成
					CMissile::Create(D3DXVECTOR3(m_pos.x, m_pos.y + 50.0f, m_pos.z),
						D3DXVECTOR3(MISSILE_SIZE_X, MISSILE_SIZE_Y, MISSILE_SIZE_Z),
						CMissile::MISSILE_USER_PL2, MISSILE_SPPED);

					//発射音
					pSound->Play(CSound::SOUND_LABEL_SE_BULLET2);

					if (m_pCharge != NULL)
					{
						//弾うったらゲージを減らす
						m_pCharge->Reduce(PLAYER_BOMB, true, 1);
					}
				}
				break;
			}

		}
	}
}

//=============================================================================
// ボムの処理
//=============================================================================
void CPlayer::bomb(void)
{
	if (m_nBombInter >= PLAYER_BOMB_INTER)
	{
		// キーボード更新
		CInputKeyboard *pKeyboard = CManager::GetKeyboard();
		CSound *pSound = CManager::GetSound();

		// Lキーを押したとき・コントローラのR1を押したとき
		if (CManager::GetJoypad()->GetJoystickTrigger(CInputJoypad::JOY_BUTTON_R2_TRIGGER, m_nPlayerNum) && m_bJump == false
			|| pKeyboard->GetTrigger(DIK_V) && m_bJump == false)
		{
			m_nBombInter = 0;
			switch (m_nPlayerNum)
			{
			case 0:
				if (CCharge::GetCharge(0) > PLAYER_BOMB)
				{
					//バレットの生成
					CBomb::Create(D3DXVECTOR3(m_pos.x, m_pos.y + 50.0f, m_pos.z),
						D3DXVECTOR3(0.0f, BOMB_MOVE_Y, 0.0f),
						D3DXVECTOR3(BOMB_SIZE_X, BOMB_SIZE_Y, BOMB_SIZE_Z), CBullet2::BULLET2_USER_PL1);

					//発射音
					pSound->Play(CSound::SOUND_LABEL_SE_BULLET2);
					//弾うったらゲージを減らす

					if (m_pCharge != NULL)
					{
						m_pCharge->Reduce(PLAYER_BOMB, true, 0);
					}
				}
				break;

			case 1:

				if (CCharge::GetCharge(1) > PLAYER_BOMB)
				{
					//バレットの生成
					CBomb::Create(D3DXVECTOR3(m_pos.x, m_pos.y + 50.0f, m_pos.z),
						D3DXVECTOR3(0.0f, BOMB_MOVE_Y, BOMB_SPEED),
						D3DXVECTOR3(BOMB_SIZE_X, BOMB_SIZE_Y, BOMB_SIZE_Z), CBullet2::BULLET2_USER_PL2);

					//発射音
					pSound->Play(CSound::SOUND_LABEL_SE_BULLET2);

					if (m_pCharge != NULL)
					{
						//弾うったらゲージを減らす
						m_pCharge->Reduce(PLAYER_BOMB, true, 1);
					}
				}
				break;
			}
		}
	}
}

//=============================================================================
// レーザー
//=============================================================================
void CPlayer::Laser(void)
{
	if (m_pLife[0] != NULL)
	{
		float fSizeX = m_pLife[0]->GetSize().x;

		if (m_nLaserInter >= PLAYER_LASER_INTER && fSizeX <= 200.0f)
		{
			// キーボード更新
			CInputKeyboard *pKeyboard = CManager::GetKeyboard();

			// Mキーを押したとき・コントローラのL2を押したとき
			if (CManager::GetJoypad()->GetJoystickTrigger(CInputJoypad::JOY_BUTTON_L2_TRIGGER, m_nPlayerNum) && m_bJump == false
				|| pKeyboard->GetTrigger(DIK_M) && m_bJump == false)
			{
				SetMotion(MOTION_BEAM);

				m_nLaserInter = 0;

				switch (m_nPlayerNum)
				{
				case 0:
					m_fAngle = atan2f(m_pos.x - CGame::GetPlayer(1)->GetPos().x, m_pos.z - CGame::GetPlayer(1)->GetPos().z);

					if (m_fAngle < 0)
					{
						m_fAngle = D3DXToRadian(360.0f + D3DXToDegree(m_fAngle));
					}

					m_rotDest.y = m_fAngle;

					while (m_rotDest.y - m_rot.y > D3DXToRadian(180))
					{
						m_rotDest.y -= D3DXToRadian(360);
					}

					while (m_rotDest.y - m_rot.y < D3DXToRadian(-180))
					{
						m_rotDest.y += D3DXToRadian(360);
					}

					m_rot += (m_rotDest - m_rot);

					//バレットの生成
					CLaser::Create(D3DXVECTOR3(m_pos.x, m_pos.y + 200.0f, m_pos.z),
						D3DXVECTOR3(0.0f, 0.0f, -LASER_SPEED),
						D3DXVECTOR3(0.0f, m_rot.y, 0.0f),
						D3DXVECTOR3(LASER_SIZE_X, LASER_SIZE_Y, LASER_SIZE_Z),
						CBullet2::BULLET2_USER_PL1);

					//弾うったらゲージを減らす
					if (m_pCharge != NULL)
					{
						m_pCharge->Reduce(150, true, 0);
					}
					break;

				case 1:
					m_fAngle = atan2f(m_pos.x - CGame::GetPlayer(0)->GetPos().x, m_pos.z - CGame::GetPlayer(0)->GetPos().z);

					if (m_fAngle < 0)
					{
						m_fAngle = D3DXToRadian(360.0f + D3DXToDegree(m_fAngle));
					}

					m_rotDest.y = m_fAngle;

					while (m_rotDest.y - m_rot.y > D3DXToRadian(180))
					{
						m_rotDest.y -= D3DXToRadian(360);
					}

					while (m_rotDest.y - m_rot.y < D3DXToRadian(-180))
					{
						m_rotDest.y += D3DXToRadian(360);
					}

					m_rot += (m_rotDest - m_rot);

					//バレットの生成

					CLaser::Create(D3DXVECTOR3(m_pos.x, m_pos.y + 200.0f, m_pos.z),
						D3DXVECTOR3(0.0f, 0.0f, LASER_SPEED),
						D3DXVECTOR3(0.0f, m_rot.y, 0.0f),

						D3DXVECTOR3(LASER_SIZE_X, LASER_SIZE_Y, LASER_SIZE_Z),
						CBullet2::BULLET2_USER_PL2);

					//弾うったらゲージを減らす
					if (m_pCharge != NULL)
					{

						m_pCharge->Reduce(150, true, 1);
					}
					break;
				}
			}
		}
	}
}

//=============================================================================
// 範囲外に行かないようにする処理
//=============================================================================

void CPlayer::MapLimit(void)
{
	//右
	if (m_pos.x > MAP_LIMIT)
	{
		m_pos.x = MAP_LIMIT;
	}

	//左
	if (m_pos.x <-MAP_LIMIT)
	{
		m_pos.x = -MAP_LIMIT;
	}

	//奥
	if (m_pos.z > MAP_LIMIT)
	{
		m_pos.z = MAP_LIMIT;
	}

	//手前
	if (m_pos.z <-MAP_LIMIT)
	{
		m_pos.z = -MAP_LIMIT;
	}
}

//=============================================================================
// ブロックの上に乗ったとき
//=============================================================================
void CPlayer::BlockUp(void)
{
	if (m_bJump == true)
	{
		//着地モーションの再生
		SetMotion(MOTION_LANDING);
	}
	m_move.y = 0.0f;
	m_bJump = false;

	if (m_bFall == true)
	{
		// 急降下を使用していない状態にする
		m_bFall = false;
	}
}

//=============================================================================
// ファイル読み込み処理
//=============================================================================
HRESULT CPlayer::ReadFile(void)
{
	FILE *pFile = NULL;	//FILEポインタ
	char aHeadData[1024];
	char aModeName[1024];
	int nModelIndex = 0;	//モデルのインデックス
	int nMotionType = 0;	//モーションのタイプ
	int nKeyNum = 0;		//キー番号
	int nMotionNum = 0;		//モーション番号
	if (m_nPlayerNum == 0)
	{
		//ファイルオープン
		pFile = fopen(LBX_XFAILE_NAME, "r");
	}
	else
	{
		//ファイルオープン
		pFile = fopen(GANDAMU_XFAILE_NAME, "r");
	}

	if (pFile != NULL)
	{
		do
		{
			//一列読み込んでモード情報を抽出
			fgets(aHeadData, sizeof(aHeadData), pFile);
			sscanf(aHeadData, "%s", aModeName);

			if (strcmp(aModeName, "MODEL_FILENAME") == 0)
			{
				//Xファイルの名前
				sscanf(aHeadData, "%*s %*s %s %*s %*s", g_modelfile[nModelIndex].xFileName);

				//インデックスを１つ進める
				nModelIndex++;
			}

			if (strcmp(aModeName, "CHARACTERSET") == 0)
			{
				//インデックスを最初に戻す
				nModelIndex = 0;

				//END_MOTIONSETを読み込むまで繰り返す
				while (strcmp(aModeName, "END_CHARACTERSET") != 0)
				{
					//一列読み込んでモード情報を抽出
					fgets(aHeadData, sizeof(aHeadData), pFile);
					sscanf(aHeadData, "%s", aModeName);

					if (strcmp(aModeName, "PARTSSET") == 0)
					{
						//END_PARTSSETを読み込むまで繰り返す
						while (strcmp(aModeName, "END_PARTSSET") != 0)
						{
							//一列読み込んでモード情報を抽出
							fgets(aHeadData, sizeof(aHeadData), pFile);
							sscanf(aHeadData, "%s", aModeName);

							if (strcmp(aModeName, "PARENT") == 0)
							{
								//親子情報の設定
								sscanf(aHeadData, "%*s %*s %d", &g_modelfile[nModelIndex].nParent);
							}
							if (strcmp(aModeName, "POS") == 0)
							{
								//位置の設定
								sscanf(aHeadData, "%*s %*s %f %f %f", &g_modelfile[nModelIndex].offsetPos.x,
									&g_modelfile[nModelIndex].offsetPos.y, &g_modelfile[nModelIndex].offsetPos.z);
							}
							if (strcmp(aModeName, "ROT") == 0)
							{
								//向きの設定
								sscanf(aHeadData, "%*s %*s %f %f %f", &g_modelfile[nModelIndex].offsetRot.x,
									&g_modelfile[nModelIndex].offsetRot.y, &g_modelfile[nModelIndex].offsetRot.z);
							}
						}
						//インデックスを１つ進める
						nModelIndex++;
					}
				}
			}

			//モーションセット
			if (strcmp(aModeName, "MOTIONSET") == 0)
			{
				//END_MOTIONSETを読み込むまで繰り返す
				while (strcmp(aModeName, "END_MOTIONSET") != 0)
				{
					//一列読み込んでモード情報を抽出
					fgets(aHeadData, sizeof(aHeadData), pFile);
					sscanf(aHeadData, "%s", aModeName);

					//ループ情報の取得
					if (strcmp(aModeName, "LOOP") == 0)
					{
						sscanf(aHeadData, "%*s %*s %d", (int*)&m_Motion[nMotionType].bLoop);
					}

					//キー情報の取得
					if (strcmp(aModeName, "NUM_KEY") == 0)
					{
						sscanf(aHeadData, "%*s %*s %d", (int*)&m_Motion[nMotionType].nNumKey);
					}

					if (strcmp(aModeName, "KEYSET") == 0)
					{
						//END_KEYSETになるまで繰り返す
						while (strcmp(aModeName, "END_KEYSET") != 0)
						{
							//一列読み込んでモード情報を抽出
							fgets(aHeadData, sizeof(aHeadData), pFile);
							sscanf(aHeadData, "%s", aModeName);

							//フレーム数の取得
							if (strcmp(aModeName, "FRAME") == 0)
							{
								sscanf(aHeadData, "%*s %*s %d", &m_Motion[nMotionType].aKeyInfo[nMotionNum].nFrame);
							}

							//各キーのオフセット情報の取得
							if (strcmp(aModeName, "KEY") == 0)
							{
								//END_KEYになるまで繰り返す
								while (strcmp(aModeName, "END_KEY") != 0)
								{
									//一列読み込んでモード情報を抽出
									fgets(aHeadData, sizeof(aHeadData), pFile);
									sscanf(aHeadData, "%s", aModeName);

									//位置の取得
									if (strcmp(aModeName, "POS") == 0)
									{
										sscanf(aHeadData, "%*s %*s %f %f %f",
											&m_Motion[nMotionType].aKeyInfo[nMotionNum].aKey[nKeyNum].fPosX,
											&m_Motion[nMotionType].aKeyInfo[nMotionNum].aKey[nKeyNum].fPosY,
											&m_Motion[nMotionType].aKeyInfo[nMotionNum].aKey[nKeyNum].fPosZ);
									}

									//向きの取得
									if (strcmp(aModeName, "ROT") == 0)
									{
										sscanf(aHeadData, "%*s %*s %f %f %f",
											&m_Motion[nMotionType].aKeyInfo[nMotionNum].aKey[nKeyNum].fRotX,
											&m_Motion[nMotionType].aKeyInfo[nMotionNum].aKey[nKeyNum].fRotY,
											&m_Motion[nMotionType].aKeyInfo[nMotionNum].aKey[nKeyNum].fRotZ);
									}
								}
								//読み終わったらカウントを1つ進める
								nKeyNum++;
							}
						}
						nKeyNum = 0;
						nMotionNum++;
					}
				}
				nMotionNum = 0;
				nMotionType++;
			}
		} while (strcmp(aModeName, "END_SCRIPT") != 0);

		//ファイルクローズ
		fclose(pFile);

		return S_OK;
	}
	else
	{
		//失敗した場合メッセージボックスを表示
		MessageBox(NULL, "モーションファイルを開くのに失敗しました", "警告", MB_OK | MB_ICONEXCLAMATION);

		return	E_FAIL;
	}
}

void CPlayer::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=============================================================================
// 勝ち負けロゴフラグの設定
//=============================================================================
void CPlayer::SetWinToLose(bool bFlag)
{
	m_bWinLose = bFlag;
}

void CPlayer::SetState(PLAYER_STATE state)
{
	m_state = state;
}


bool CPlayer::GetSetWinLose(void)
{
	return m_bWinLose;
}

//=============================================================================
// ライフの情報
//=============================================================================
CLife * CPlayer::GetLife(int nNumber)
{
	return m_pLife[nNumber];
}

//=============================================================================
// 座標の情報
//=============================================================================
D3DXVECTOR3 CPlayer::GetPos(void)
{
	return m_pos;
}

//=============================================================================
// 古い座標情報
//=============================================================================
D3DXVECTOR3 CPlayer::GetOldPos(void)
{
	return m_OldPos;
}

//=============================================================================
// 移動量の設定
//=============================================================================
void CPlayer::SetMove(D3DXVECTOR3 move)
{
	m_move = move;
}

D3DXVECTOR3 CPlayer::GetMove(void)
{
	return m_move;
}

D3DXVECTOR3 CPlayer::GetRot(void)
{
	return m_rot;
}

//=============================================================================
// チャージの情報
//=============================================================================
CCharge * CPlayer::GetCgarge(void)
{
	return m_pCharge;
}
//=============================================================================
// エンド情報
//=============================================================================
bool CPlayer::GetEnd(void)
{
	return m_bEnd;
}

//=============================================================================
// プレイヤー状態の情報
//=============================================================================
CPlayer::PLAYER_STATE CPlayer::GetState(void)
{
	return m_state;
}

bool CPlayer::GetJump(void)
{
	return m_bJump;
}

//=============================================================================
// 引き分け情報
//=============================================================================
bool CPlayer::GetDraw(void)
{
	return m_bDraw;
}

//=============================================================================
// 無敵フラグの情報
//=============================================================================
bool CPlayer::GetArmor(void)
{
	return m_bArmor;
}

//=============================================================================
// 勝ちのフラグ
//=============================================================================
bool CPlayer::GetWin(void)
{
	return m_bWin;
}
