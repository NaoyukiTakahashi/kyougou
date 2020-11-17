//=============================================================================
//
// Author : Konishi Yuuto
//=============================================================================
// インクルード
//=============================================================================
#include "missile.h"
#include "modelanime.h"
#include "manager.h"
#include "renderer.h"
#include "effect.h"
#include "game.h"
#include "player.h"
#include "life.h"
#include "camera.h"
#include "2d_explosion.h"
#include "shock.h"
#include "model.h"
#include "bill.h"
#include "splash.h"
#include "missile.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define MISSILE_XFAILE_NAME "data/Text/motionMissile.txt"	//ミサイルのデータファイルパス
#define FOLLOW_TIME_MISSILE		(110)						// ミサイルの追従
#define MISSILE_LIFE			(300)						// ミサイルのライフ
#define MISSILE_ATTACK			(100)

//=============================================================================
//グローバル変数宣言
//=============================================================================
CMissile::MODELFILLEs g_modelfile[MAX_MODEL_MISSILE_PARTS];	//モデルファイルのグローバル変数

//=============================================================================
//ミサイルクラスのコンストラクタ
//=============================================================================
CMissile::CMissile(int nPriority) : CScene(nPriority)
{
	//各メンバ変数のクリア
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nLife = MISSILE_LIFE;
	memset(m_apModelAnime, 0, sizeof(m_apModelAnime));
	m_nCounter = 0;
	m_user = MISSILE_USER_NONE;
	m_pTargetPL = NULL;
	m_nDamage = 0;
	m_fSpeed = 0.0f;
	m_fHeight = 0.0f;
}

//=============================================================================
//ミサイルクラスのデストラクタ
//=============================================================================
CMissile::~CMissile()
{
}

//=============================================================================
//ミサイルクラスのクリエイト処理
//=============================================================================
CMissile * CMissile::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size,
	const MISSILE_USER user, float fSpeed)
{
	//ミサイルクラスのポインタ変数
	CMissile *pMissile = NULL;

	//インスタンス生成
	pMissile = new CMissile;

	//メモリが確保できていたら
	if (pMissile != NULL)
	{
		pMissile->m_fSpeed = fSpeed;
		pMissile->m_user = user;

		//初期化処理呼び出し
		pMissile->Init(pos, size);
		//pMissile->SetRot(rot);
		//pMissile->SetMove(move);
	}
	else
	{
		return NULL;
	}

	return pMissile;
}

//=============================================================================
//ミサイルクラスの初期化処理
//=============================================================================
HRESULT CMissile::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	//位置の設定
	m_pos = pos;

	//向きの設定
	m_size = size;

	m_nDamage = MISSILE_ATTACK;

	//ファイル読み込み
	if (FAILED(ReadFile()))
	{
		return E_FAIL;
	}

	//モデルパーツ数分繰り返す
	for (int nCntModel = 0; nCntModel < MAX_MODEL_MISSILE_PARTS; nCntModel++)
	{
		//モデルの生成
		m_apModelAnime[nCntModel] = CModelAnime::Create(g_modelfile[nCntModel].xFileName,
			g_modelfile[nCntModel].offsetPos, g_modelfile[nCntModel].offsetRot);

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

	switch (m_user)
	{
	case MISSILE_USER_PL1:
		//2Pの情報取得
		m_pTargetPL = CGame::GetPlayer(1);
		break;
	case MISSILE_USER_PL2:
		//1Pの情報取得
		m_pTargetPL = CGame::GetPlayer(0);

		break;
	}

	return S_OK;
}

//=============================================================================
//ミサイルクラスの終了処理
//=============================================================================
void CMissile::Uninit(void)
{
	//モデルパーツ数分繰り返す
	for (int nCntModel = 0; nCntModel < MAX_MODEL_MISSILE_PARTS; nCntModel++)
	{
		if (m_apModelAnime[nCntModel] != NULL)
		{
			//階層モデルクラスの終了処理
			m_apModelAnime[nCntModel]->Uninit();

			//メモリの削除
			delete m_apModelAnime[nCntModel];

			//メモリのクリア
			m_apModelAnime[nCntModel] = NULL;
		}
	}

	//オブジェクトの破棄
	SetDeathFlag();
}

//=============================================================================
//ミサイルクラスの更新処理
//=============================================================================
void CMissile::Update(void)
{	//位置の取得
	m_nCounter++;

	// 通常の場合
	if (m_nCounter <= FOLLOW_TIME_MISSILE)
	{
		m_rot.y = atan2f(m_pos.x - CGame::GetPlayer(1)->GetPos().x, m_pos.z - CGame::GetPlayer(1)->GetPos().z);

		if (m_rot.y < 0)
		{
			m_rot.y = D3DXToRadian(360.0f + D3DXToDegree(m_rot.y));
		}

		if (m_pTargetPL != NULL)
		{
			//移動量の計算
			m_move = VectorMath(D3DXVECTOR3(
				m_pTargetPL->GetPos().x, m_pTargetPL->GetPos().y + 200.0f, m_pTargetPL->GetPos().z), m_fSpeed);
		
			// 高さの調整
			m_move.y = m_fHeight;
		}
	}


	//移動量を位置に与える
	m_pos += m_move;

	//ライフの設定
	m_nLife--;

	if (Collision() == true)
	{
		// 体力を0
		m_nLife = 0;
	}

	m_pos.y += 0.05f;

	// エフェクト生成
	CEffect::Create(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(EFFECT_SIZE_X, EFFECT_SIZE_Y, 0.0f), D3DXCOLOR(0.3f, 0.3f, 1.0f, 1.0f), EFFECT_LIFE);

	//ライフが0以下の時
	if (m_nLife <= 0)
	{
		// 衝撃を生成
		CShock::Create(D3DXVECTOR3(m_pos.x, 0.0f, m_pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXVECTOR3(SHOCK_SIZE_X, SHOCK_SIZE_Y, SHOCK_SIZE_Z));

		//終了処理呼び出し
		Uninit();
		return;
	}
}

//=============================================================================
//ミサイルクラスの描画処理
//=============================================================================
void CMissile::Draw(void)
{
	//デバイス情報の取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxScale;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// サイズを反映
	D3DXMatrixScaling(&mtxScale, m_size.x, m_size.y, m_size.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int nCntModelNum = 0; nCntModelNum < MAX_MODEL_MISSILE_PARTS; nCntModelNum++)
	{
		if (m_apModelAnime[nCntModelNum] != NULL)
		{
			//階層モデルクラスの描画処理
			m_apModelAnime[nCntModelNum]->Draw();
		}
	}
}

//=============================================================================
//バレットクラスの当たり判定
//=============================================================================
bool CMissile::Collision(void)
{

	if (m_pTargetPL != NULL)
	{
		//位置の取得
		D3DXVECTOR3 targetPos = m_pTargetPL->GetPos();


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
					if (m_pTargetPL->GetArmor() == false)
					{
						bool bLife = m_pTargetPL->GetLife(0)->GetbLife();

						if (bLife == false)
						{
							m_pTargetPL->GetLife(nCount)->Decrease(m_nDamage, m_user, true);
							m_pTargetPL->GetLife(1)->Decrease(m_nDamage, m_user, true);

							m_pTargetPL->SetMotion(MOTION_DAMAGE);
						}


						// プレイヤー情報の取得
						switch (m_user)
						{
						case MISSILE_USER_PL1:
							CGame::GetCamera(1)->SetTarget(false);
							break;
						case MISSILE_USER_PL2:
							CGame::GetCamera(0)->SetTarget(false);
							break;
						}
					}
					// 爆発生成
					C2dExplosion::Create(D3DXVECTOR3(m_pTargetPL->GetPos().x, m_pos.y, m_pTargetPL->GetPos().z),
						D3DXVECTOR3(EXPLOSION_SIZE_X_2D, EXPLOSION_SIZE_Y_2D, EXPLOSION_SIZE_Z_2D));


				}
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
				if (type == CModel::MODEL_TYPE_OBJECT)
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
D3DXVECTOR3 CMissile::VectorMath(D3DXVECTOR3 TargetPos, float fSpeed)
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
//ミサイルクラスの向きの設定処理
//=============================================================================
void CMissile::SetRot(D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//=============================================================================
//ミサイルクラスの移動量の設定処理
//=============================================================================
void CMissile::SetMove(D3DXVECTOR3 move)
{
	m_move = move;
}

//=============================================================================
//ミサイルクラスのファイル読み込み処理
//=============================================================================
HRESULT CMissile::ReadFile(void)
{
	FILE *pFile = NULL;	//FILEポインタ
	char aHeadData[1024];
	char aModeName[1024];
	int nModelIndex = 0;

	//ファイルオープン
	pFile = fopen(MISSILE_XFAILE_NAME, "r");

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
		} while (strcmp(aModeName, "END_SCRIPT") != 0);

		//ファイルクローズ
		fclose(pFile);

		return S_OK;
	}
	else
	{
		//失敗した場合メッセージボックスを表示
		MessageBox(NULL, "モーションファイルを開くのに失敗しました", "ミサイルクラスでのエラー", MB_OK | MB_ICONEXCLAMATION);

		return	E_FAIL;
	}}
