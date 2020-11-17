//=============================================================================
//
// 弾の処理(モデル) [bullet2.cpp]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "bullet2.h"
#include "player.h"
#include "game.h"
#include "life.h"
#include "2d_explosion.h"
#include "shock.h"
#include "explosion.h"
#include "bill.h"
#include "splash.h"
#include "camera.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define BULLET2_ATK			(20)		// 攻撃力
#define FOLLOW_TIME_NONE	(30)		// 通常弾の追従タイム
#define FOLLOW_TIME_BOMB	(125)		// ボムの追従タイム

//=============================================================================
// コンストラクタ
//=============================================================================
CBullet2::CBullet2()
{
	// 各メンバ変数のクリア
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// サイズ
	m_user = BULLET2_USER_NONE;					// 使用者
	m_type = BULLET2_TYPE_NONE;					// タイプ
	m_nAtk = 0;									// 攻撃力
	m_nLife = 0;								// ライフ
	m_nCounter = 0;
	m_pTargetPL = NULL;							//敵プレイヤーのポインタ
	m_fSpeed = 0.0f;
	m_fHeight = 0.0f;
	m_nDamage = 20;
}

//=============================================================================
//バレットクラスのデストラクタ
//=============================================================================
CBullet2::~CBullet2()
{
}

//=============================================================================
//バレットクラスのクリエイト処理
//=============================================================================
CBullet2 * CBullet2::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, 
	const BULLET2_USER user, float fSpeed)
{
	CBullet2 *pBullet2 = NULL;

	//メモリの確保
	pBullet2 = new CBullet2;

	//メモリが確保できていたら
	if (pBullet2)
	{
		//初期化処理呼び出し
		pBullet2->Init(pos, size, user, fSpeed);		// 初期化情報
	}
	else
	{
		return NULL;
	}

	return pBullet2;
}

//=============================================================================
//バレットクラスの初期化処理
//=============================================================================
HRESULT CBullet2::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, 
	const BULLET2_USER user, float fSpeed)
{
	//位置の設定
	m_pos = pos;

	//サイズの設定
	m_size = size;

	// スピード
	m_fSpeed = fSpeed;

	// サイズの設定
	SetSize(size);

	//使用者の設定
	m_user = user;

	//攻撃力の設定
	m_nAtk = BULLET2_ATK;

	//初期化初期化処理
	CModel::Init(pos, m_size);

	switch (m_user)
	{
	case BULLET2_USER_PL1:
		//2Pの情報取得
		m_pTargetPL = CGame::GetPlayer(1);

		break;
	case BULLET2_USER_PL2:
		//1Pの情報取得
		m_pTargetPL = CGame::GetPlayer(0);

		break;
	}

	return S_OK;
}

//=============================================================================
//バレットクラスの終了処理
//=============================================================================
void CBullet2::Uninit(void)
{
	//終了処理
	CModel::Uninit();
}

//=============================================================================
//バレットクラスの更新処理
//=============================================================================
void CBullet2::Update(void)
{
	//位置の取得
	m_pos = GetPos();
	m_nCounter++;

	switch (m_type)
	{
	case BULLET2_TYPE_NONE:
		// 通常の場合
		if (m_nCounter <= FOLLOW_TIME_NONE)
		{
			//移動量の計算
			m_move = VectorMath(D3DXVECTOR3(
				m_pTargetPL->GetPos().x, m_pTargetPL->GetPos().y + 200.0f, m_pTargetPL->GetPos().z), m_fSpeed);
		}
		break;

	case BULLET2_TYPE_BOMB:
		// ボムの時
		if (m_nCounter <= FOLLOW_TIME_BOMB)
		{
			m_move = VectorMath(D3DXVECTOR3(
				m_pTargetPL->GetPos().x, m_pTargetPL->GetPos().y + 200.0f, m_pTargetPL->GetPos().z),
				m_fSpeed);
		}

		// 高さの調整
		m_move.y = m_fHeight;
		break;

	case BULLET2_TYPE_LASER:
		// 通常の場合
		if (m_nCounter <= FOLLOW_TIME_NONE)
		{
			//移動量の計算
			m_move = VectorMath(m_pTargetPL->GetPos(), m_fSpeed);
		}
		break;

	}

	//移動量を位置に与える
	m_pos += m_move;

	//ライフの設定
	m_nLife--;

	//位置の設定
	SetPos(m_pos);

	if (Collision() == true)
	{
		// 体力を0
		m_nLife = 0;
	}

	//ライフが0以下の時
	if (m_nLife <= 0)
	{
		switch (m_type)
		{
		case BULLET2_TYPE_NONE:
			break;
		case BULLET2_TYPE_BOMB:
			CExplosion::Create(D3DXVECTOR3(m_pos.x, 0.0f, m_pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(EXPLOSION_SIZE_X, EXPLOSION_SIZE_Y, EXPLOSION_SIZE_Z));
			break;
		default:

			break;
		}

		//終了処理呼び出し
		Uninit();
		return;
	}
}

//=============================================================================
//バレットクラスの描画処理
//=============================================================================
void CBullet2::Draw(void)
{
	//描画処理
	CModel::Draw();
}

//=============================================================================
//バレットクラスの当たり判定
//=============================================================================
bool CBullet2::Collision(void)
{
	CPlayer * pPlayer = NULL;

	switch (m_user)
	{
	case BULLET2_USER_PL1:

		//プレイヤークラスへキャスト
		pPlayer = CGame::GetPlayer(1);
		break;

	case BULLET2_USER_PL2:

		//プレイヤークラスへキャスト
		pPlayer = CGame::GetPlayer(0);
		break;

	}

	//位置の取得
	D3DXVECTOR3 targetPos = pPlayer->GetPos();
	bool bLife = pPlayer->GetLife(0)->GetbLife();

		// 当たり判定
	if (targetPos.x + PLAYER_COLLISION_X / 2 >= m_pos.x - 50.0f &&
		targetPos.x - PLAYER_COLLISION_X / 2 <= m_pos.x + 50.0f &&
		targetPos.y + PLAYER_COLLISION_Y >= m_pos.y - 50.0f &&
		targetPos.y - 0.0f <= m_pos.y + 50.0f &&
		targetPos.z + PLAYER_COLLISION_Z / 2 >= m_pos.z - 50.0f &&
		targetPos.z - PLAYER_COLLISION_Z / 2 <= m_pos.z + 50.0f)
	{
		for (int nCount = 0; nCount < LIFE_NUM; nCount++)
		{
			//　プレイヤーのライフを減らす
			if (m_pTargetPL != NULL)
			{
				switch (m_type)
				{
				case BULLET2_TYPE_NONE:
					// 通常の場合
					m_nDamage = 20;
					break;
				case BULLET2_TYPE_BOMB:
					// ボムの時
					m_nDamage = 100;
					break;
				case BULLET2_TYPE_LASER:
					// ボムの時
					m_nDamage = 160;
					break;
				}

				if (pPlayer->GetArmor() == false)
				{
					if (bLife == false)
					{
						m_pTargetPL->GetLife(nCount)->Decrease(m_nDamage, m_user, true);
						m_pTargetPL->GetLife(1)->Decrease(m_nDamage, m_user, true);

						if (m_type != BULLET2_TYPE_NONE)
						{
							m_pTargetPL->SetMotion(MOTION_DAMAGE);
						}
					}

					// プレイヤー情報の取得
					switch (m_user)
					{
					case BULLET2_USER_PL1:
						CGame::GetCamera(1)->SetTarget(false);
						break;
					case BULLET2_USER_PL2:
						CGame::GetCamera(0)->SetTarget(false);
						break;
					}




				}// 爆発生成
				C2dExplosion::Create(D3DXVECTOR3(m_pTargetPL->GetPos().x, m_pos.y, m_pTargetPL->GetPos().z),
					D3DXVECTOR3(EXPLOSION_SIZE_X_2D, EXPLOSION_SIZE_Y_2D, EXPLOSION_SIZE_Z_2D));
			}
			m_nLife = 0;
			return true;
		}
	}

	for (int nCount = 0; nCount < PRIORITY_MAX; nCount++)
	{
		//先頭情報を取得
		CScene *pScene = CScene::GetTop(nCount);

		//NULLになるまで繰り返す
		while (pScene)
		{
			// オブジェクトタイプを取得
			OBJTYPE type = pScene->GetObjType();

			// モデルなら
			if (type == OBJTYPE_MODEL)
			{
				// キャスト
				CModel *pModel = (CModel*)pScene;
				CModel::MODEL_TYPE type = pModel->GetType();

				// 建物なら
				if (type == MODEL_TYPE_OBJECT)
				{
					//位置の取得
					D3DXVECTOR3 targetPos = pModel->GetPos();

					// 当たり判定
					if (targetPos.x >= m_pos.x - BILL_COLLISION_SIZE_X / 2 &&
						targetPos.x <= m_pos.x + BILL_COLLISION_SIZE_X / 2 &&
						targetPos.y >= m_pos.y - BILL_COLLISION_SIZE_Y / 2 &&
						targetPos.y <= m_pos.y + BILL_COLLISION_SIZE_Y / 2 &&
						targetPos.z >= m_pos.z - BILL_COLLISION_SIZE_Z / 2 &&
						targetPos.z <= m_pos.z + BILL_COLLISION_SIZE_Z / 2)
					{
						for (int nCount = 0; nCount < LIFE_NUM; nCount++)
						{
							// 爆発生成
							CSplash::Create(m_pos,
								D3DXVECTOR3(EXPLOSION_SIZE_X_2D, EXPLOSION_SIZE_Y_2D, EXPLOSION_SIZE_Z_2D));
						}

						m_nLife = 0;
						return true;
					}
				}
			}
			//次の情報を取得
			pScene = pScene->GetNext();
		}
	}
	
	return false;
}

//=============================================================================
//バレットクラスののベクトル計算処理
//=============================================================================
D3DXVECTOR3 CBullet2::VectorMath(D3DXVECTOR3 TargetPos, float fSpeed)
{

	//2点間のベクトルを求める（終点[目標地点] - 始点[自身の位置]）
	D3DXVECTOR3 Vector = TargetPos - m_pos;

	//ベクトルの大きさを求める(√c^2 = a^2 * b^2)
	float fVectorSize = D3DXVec3Length(&Vector);

	//単位ベクトル用変数
	D3DXVECTOR3 UnitVector;

	//単位ベクトルを求める(元のベクトル / ベクトルの大きさ)
	D3DXVec3Normalize(&UnitVector, &Vector);

	//単位ベクトルを速度倍にして返す(UnitVector * fSpeed)
	return	UnitVector * fSpeed;
}


//=============================================================================
// 体力の設定
//=============================================================================
void CBullet2::SetLife(int nLife)
{
	m_nLife = nLife;
}

//=============================================================================
// 移動量の設定
//=============================================================================
void CBullet2::SetMove(D3DXVECTOR3 move)
{
	m_move = move;
}

//=============================================================================
// タイプの設定
//=============================================================================
void CBullet2::SetType(BULLET2_TYPE type)
{
	m_type = type;
}

//=============================================================================
// 高さの設定
//=============================================================================
void CBullet2::SetHeight(float fHeight)
{
	m_fHeight = fHeight;
}

//=============================================================================
// 移動量情報
//=============================================================================
D3DXVECTOR3 CBullet2::GetMove(void)
{
	return m_move;
}

//=============================================================================
// 高さの情報
//=============================================================================
float CBullet2::GetHeight(void)
{
	return m_fHeight;
}
