//=============================================================================
//
// ビル処理 [bill.cpp]
// Author : 佐藤颯紀
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "bill.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "game.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_VERTEX		(8)		// 頂点数

//=============================================================================
// static初期化
//=============================================================================
LPD3DXMESH CBill::m_pMesh = NULL;			// メッシュ情報へのポインタ
LPD3DXBUFFER CBill::m_pBuffMat = NULL;	// マテリアル情報へのポインタ
DWORD CBill::m_nNumMat = 0;				// マテリアル情報の数
LPDIRECT3DTEXTURE9 CBill::m_apTexture[10] = {};

//=============================================================================
// クリエイト
//=============================================================================
CBill * CBill::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// 初期化処理
	CBill *pBill= new CBill;

	// 初期化処理
	pBill->Init(pos, size);

	return pBill;
}

//=============================================================================
// モデルロード
//=============================================================================
HRESULT CBill::LoadModel(void)
{
	//デバイス情報の取得
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	// モデルの生成
	D3DXLoadMeshFromX("data/model/Building.x", D3DXMESH_SYSTEMMEM,
		pD3DDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);

	//マテリアル情報の解析
	D3DXMATERIAL *materials = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		//ファイルネームの取得
		char *aXfileName = materials[nCntMat].pTextureFilename;

		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pD3DDevice, aXfileName, &m_apTexture[nCntMat]);
	}
	// 正常終了
	return S_OK;
}

//=============================================================================
// モデルアンロード
//=============================================================================
void CBill::Unload(void)
{
	//メッシュの破棄
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}
	//マテリアルの破棄
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}

	for (int nCntTexture = 0; nCntTexture < 10; nCntTexture++)
	{
		if (m_apTexture[nCntTexture] != NULL)
		{
			m_apTexture[nCntTexture]->Release();
			m_apTexture[nCntTexture] = NULL;
		}
	}
}

//=============================================================================
// コンストラクタ
//=============================================================================
CBill::CBill()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fAngle = 0.0f;
}

//=============================================================================
// デストラクタ
//=============================================================================
CBill::~CBill()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBill::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// モデルの情報を設定
	MODEL model;
	model.dwNumMat = m_nNumMat;
	model.pBuffMat = m_pBuffMat;
	model.pMesh = m_pMesh;

	//モデル情報を設定
	CModel::BindModel(model);

	BindTexturePointer(m_apTexture);
	//位置の設定
	m_pos = pos;

	// 初期化	
	CModel::Init(m_pos, size);

	//rot初期化
	CModel::SetRot(m_rot);

	SetObjType(OBJTYPE_MODEL);

	// モデルの種類を設定
	SetType(MODEL_TYPE_OBJECT);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBill::Uninit(void)
{
	// 終了処理
	CModel::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CBill::Update(void)
{
	// 当たり判定
	Collision();
}

//=============================================================================
// 描画処理
//=============================================================================
void CBill::Draw(void)
{
	// 描画処理
	CModel::Draw();
}

//=============================================================================
// 当たり判定
//=============================================================================
bool CBill::Collision(void)
{
	for (int nCount = 0; nCount < MAX_PLAYER; nCount++)
	{
		// プレイヤー情報
		CPlayer *pPlayer = CGame::GetPlayer(nCount);

		if (pPlayer != NULL)
		{


			//位置の取得
			D3DXVECTOR3 targetPos = pPlayer->GetPos();

			// 当たり判定
			if (targetPos.x - PLAYER_COLLISION_X / 2 <= m_pos.x + BILL_COLLISION_SIZE_X / 2 &&
				targetPos.x + PLAYER_COLLISION_X / 2 >= m_pos.x - BILL_COLLISION_SIZE_X / 2 &&
				targetPos.y - PLAYER_COLLISION_Y / 2 <= m_pos.y + BILL_COLLISION_SIZE_Y / 2 &&
				targetPos.y + PLAYER_COLLISION_Y / 2 >= m_pos.y - BILL_COLLISION_SIZE_Y / 2 &&
				targetPos.z - PLAYER_COLLISION_Z / 2 <= m_pos.z + BILL_COLLISION_SIZE_Z / 2 &&
				targetPos.z + PLAYER_COLLISION_Z / 2 >= m_pos.z - BILL_COLLISION_SIZE_Z / 2)
			{
				D3DXVECTOR3 OldTarget = pPlayer->GetOldPos();
				D3DXVECTOR3 move = pPlayer->GetMove();

				if (targetPos.x - PLAYER_COLLISION_X / 2 <= m_pos.x + BILL_COLLISION_SIZE_X / 2 &&
					OldTarget.x + PLAYER_COLLISION_X / 2 >= m_pos.x + BILL_COLLISION_SIZE_X / 2)
				{
					// ブロックに右から当たったとき
					pPlayer->SetPos(D3DXVECTOR3(PLAYER_COLLISION_X / 2 + m_pos.x + BILL_COLLISION_SIZE_X / 2, targetPos.y, targetPos.z));	// プレイヤーを押し戻す
				}
				else if (targetPos.x + PLAYER_COLLISION_X / 2 >= m_pos.x - BILL_COLLISION_SIZE_X / 2 &&
					OldTarget.x - (PLAYER_COLLISION_X / 2) <= m_pos.x - BILL_COLLISION_SIZE_X / 2)
				{
					// ブロックに左から当たった時
					pPlayer->SetPos(D3DXVECTOR3(m_pos.x - BILL_COLLISION_SIZE_X / 2 - PLAYER_COLLISION_X / 2, targetPos.y, targetPos.z));	// プレイヤーを押し戻す
				}
				else if ((targetPos.y - PLAYER_COLLISION_Y / 2) < m_pos.y + BILL_COLLISION_SIZE_Y / 2 &&
					OldTarget.y - PLAYER_COLLISION_Y / 2 >= m_pos.y)
				{
					// ブロックに上から当たった時
					pPlayer->SetPos(D3DXVECTOR3(targetPos.x, PLAYER_COLLISION_Y / 2 + m_pos.y + BILL_COLLISION_SIZE_Y / 2, targetPos.z));
					pPlayer->BlockUp();
				}
				else if (targetPos.z - PLAYER_COLLISION_Z / 2 < m_pos.z + BILL_COLLISION_SIZE_Z / 2 &&
					OldTarget.z + PLAYER_COLLISION_Z / 2 >= m_pos.z)
				{
					// ブロックに手前から当たった時
					pPlayer->SetPos(D3DXVECTOR3(targetPos.x, targetPos.y, PLAYER_COLLISION_Z / 2 + m_pos.z + BILL_COLLISION_SIZE_Z / 2));

				}
				else if (targetPos.z + PLAYER_COLLISION_Z / 2 > m_pos.z - BILL_COLLISION_SIZE_Z / 2 &&
					OldTarget.z - PLAYER_COLLISION_Z / 2 <= m_pos.z - BILL_COLLISION_SIZE_Z / 2)
				{
					// ブロックに奥から当たった時
					pPlayer->SetPos(D3DXVECTOR3(targetPos.x, targetPos.y, m_pos.z - BILL_COLLISION_SIZE_Z / 2 - PLAYER_COLLISION_Z / 2));
				}
				else if (targetPos.y + PLAYER_COLLISION_Y / 2 > m_pos.y - BILL_COLLISION_SIZE_Y / 2 &&
					OldTarget.y + PLAYER_COLLISION_Y / 2 <= m_pos.y)
				{
					// ブロックに下から当たった時
					pPlayer->SetPos(D3DXVECTOR3(targetPos.x, m_pos.y - BILL_COLLISION_SIZE_Y / 2, targetPos.z));		// プレイヤーがブロックにめり込まないようにする
					pPlayer->SetMove(D3DXVECTOR3(move.x, 0.0f, move.z));
				}


			}
		}
	}
	//for (int nCount = 0; nCount < MAX_PLAYER; nCount++)
	//{

	//	// プレイヤー情報
	//	CPlayer *pPlayer = CGame::GetPlayer(nCount);

	//	//位置の取得
	//	D3DXVECTOR3 targetPos = pPlayer->GetPos();

	//// 頂点の位置
	//D3DXVECTOR3 VertexPos[8] =
	//{
	//	D3DXVECTOR3(m_pos.x - BILL_COLLISION_SIZE_X / 2, m_pos.y + BILL_COLLISION_SIZE_Y / 2, m_pos.z - BILL_COLLISION_SIZE_Z / 2),	// 0
	//	D3DXVECTOR3(m_pos.x - BILL_COLLISION_SIZE_X / 2, m_pos.y + BILL_COLLISION_SIZE_Y / 2, m_pos.z + BILL_COLLISION_SIZE_Z / 2),	// 1
	//	D3DXVECTOR3(m_pos.x + BILL_COLLISION_SIZE_X / 2, m_pos.y + BILL_COLLISION_SIZE_Y / 2, m_pos.z - BILL_COLLISION_SIZE_Z / 2),	// 2
	//	D3DXVECTOR3(m_pos.x + BILL_COLLISION_SIZE_X / 2, m_pos.y + BILL_COLLISION_SIZE_Y / 2, m_pos.z + BILL_COLLISION_SIZE_Z / 2),	// 3
	//	D3DXVECTOR3(m_pos.x - BILL_COLLISION_SIZE_X / 2, m_pos.y - BILL_COLLISION_SIZE_Y / 2, m_pos.z - BILL_COLLISION_SIZE_Z / 2),	// 4
	//	D3DXVECTOR3(m_pos.x - BILL_COLLISION_SIZE_X / 2, m_pos.y - BILL_COLLISION_SIZE_Y / 2, m_pos.z + BILL_COLLISION_SIZE_Z / 2),	// 5
	//	D3DXVECTOR3(m_pos.x + BILL_COLLISION_SIZE_X / 2, m_pos.y - BILL_COLLISION_SIZE_Y / 2, m_pos.z - BILL_COLLISION_SIZE_Z / 2),	// 6
	//	D3DXVECTOR3(m_pos.x + BILL_COLLISION_SIZE_X / 2, m_pos.y - BILL_COLLISION_SIZE_Y / 2, m_pos.z + BILL_COLLISION_SIZE_Z / 2),	// 7
	//};

	//// 2点間の距離
	//float Distance[8] = 
	//{
	//	sqrtf((targetPos.x - m_pos.x - BILL_COLLISION_SIZE_X / 2)*(targetPos.x - m_pos.x - BILL_COLLISION_SIZE_X / 2) +
	//	(targetPos.y - m_pos.y + BILL_COLLISION_SIZE_Y / 2)*(targetPos.x - m_pos.x + BILL_COLLISION_SIZE_Y / 2) +
	//		(targetPos.z - m_pos.z - BILL_COLLISION_SIZE_Z / 2)*(targetPos.z - m_pos.z - BILL_COLLISION_SIZE_Z / 2)),

	//	sqrtf((targetPos.x - m_pos.x - BILL_COLLISION_SIZE_X / 2)*(targetPos.x - m_pos.x - BILL_COLLISION_SIZE_X / 2) +
	//	(targetPos.y - m_pos.y + BILL_COLLISION_SIZE_Y / 2)*(targetPos.x - m_pos.x + BILL_COLLISION_SIZE_Y / 2) +
	//		(targetPos.z - m_pos.z + BILL_COLLISION_SIZE_Z / 2)*(targetPos.z - m_pos.z + BILL_COLLISION_SIZE_Z / 2)),

	//	sqrtf((targetPos.x - m_pos.x + BILL_COLLISION_SIZE_X / 2)*(targetPos.x - m_pos.x + BILL_COLLISION_SIZE_X / 2) +
	//	(targetPos.y - m_pos.y + BILL_COLLISION_SIZE_Y / 2)*(targetPos.x - m_pos.x + BILL_COLLISION_SIZE_Y / 2) +
	//		(targetPos.z - m_pos.z - BILL_COLLISION_SIZE_Z / 2)*(targetPos.z - m_pos.z - BILL_COLLISION_SIZE_Z / 2)),

	//	sqrtf((targetPos.x - m_pos.x + BILL_COLLISION_SIZE_X / 2)*(targetPos.x - m_pos.x + BILL_COLLISION_SIZE_X / 2) +
	//	(targetPos.y - m_pos.y + BILL_COLLISION_SIZE_Y / 2)*(targetPos.x - m_pos.x + BILL_COLLISION_SIZE_Y / 2) +
	//		(targetPos.z - m_pos.z + BILL_COLLISION_SIZE_Z / 2)*(targetPos.z - m_pos.z + BILL_COLLISION_SIZE_Z / 2)),

	//	sqrtf((targetPos.x - m_pos.x - BILL_COLLISION_SIZE_X / 2)*(targetPos.x - m_pos.x - BILL_COLLISION_SIZE_X / 2) +
	//	(targetPos.y - m_pos.y - BILL_COLLISION_SIZE_Y / 2)*(targetPos.x - m_pos.x - BILL_COLLISION_SIZE_Y / 2) +
	//		(targetPos.z - m_pos.z - BILL_COLLISION_SIZE_Z / 2)*(targetPos.z - m_pos.z - BILL_COLLISION_SIZE_Z / 2)),

	//	sqrtf((targetPos.x - m_pos.x - BILL_COLLISION_SIZE_X / 2)*(targetPos.x - m_pos.x - BILL_COLLISION_SIZE_X / 2) +
	//	(targetPos.y - m_pos.y - BILL_COLLISION_SIZE_Y / 2)*(targetPos.x - m_pos.x - BILL_COLLISION_SIZE_Y / 2) +
	//		(targetPos.z - m_pos.z + BILL_COLLISION_SIZE_Z / 2)*(targetPos.z - m_pos.z + BILL_COLLISION_SIZE_Z / 2)),

	//	sqrtf((targetPos.x - m_pos.x + BILL_COLLISION_SIZE_X / 2)*(targetPos.x - m_pos.x + BILL_COLLISION_SIZE_X / 2) +
	//	(targetPos.y - m_pos.y - BILL_COLLISION_SIZE_Y / 2)*(targetPos.x - m_pos.x - BILL_COLLISION_SIZE_Y / 2) +
	//		(targetPos.z - m_pos.z - BILL_COLLISION_SIZE_Z / 2)*(targetPos.z - m_pos.z - BILL_COLLISION_SIZE_Z / 2)),

	//	sqrtf((targetPos.x - m_pos.x + BILL_COLLISION_SIZE_X / 2)*(targetPos.x - m_pos.x + BILL_COLLISION_SIZE_X / 2) +
	//	(targetPos.y - m_pos.y - BILL_COLLISION_SIZE_Y / 2)*(targetPos.x - m_pos.x - BILL_COLLISION_SIZE_Y / 2) +
	//		(targetPos.z - m_pos.z + BILL_COLLISION_SIZE_Z / 2)*(targetPos.z - m_pos.z + BILL_COLLISION_SIZE_Z / 2))
	//};

	//for (int nCntVex = 0; nCntVex < MAX_VERTEX; nCntVex++)
	//{
	//	// 当たり判定
	//	if (Distance[nCntVex] <= PLAYER_RADIUS)
	//	{
	//		D3DXVec3Normalize(&VertexPos[nCntVex], &targetPos);

	//		VertexPos[nCntVex] *= PLAYER_RADIUS;
	//	}
	//}

	//	if (targetPos.x < (m_pos.x - BILL_COLLISION_SIZE_X / 2) - PLAYER_COLLISION_X / 2 ||
	//		targetPos.x > (m_pos.x + BILL_COLLISION_SIZE_X / 2) + PLAYER_COLLISION_X / 2 ||
	//		targetPos.y < (m_pos.x - BILL_COLLISION_SIZE_Y / 2) - PLAYER_COLLISION_Y / 2 ||
	//		targetPos.y > (m_pos.x + BILL_COLLISION_SIZE_Y / 2) + PLAYER_COLLISION_Y / 2 ||
	//		targetPos.z < (m_pos.x - BILL_COLLISION_SIZE_Z / 2) - PLAYER_COLLISION_Z / 2 ||
	//		targetPos.z > (m_pos.x + BILL_COLLISION_SIZE_Z / 2) + PLAYER_COLLISION_Z / 2)
	//	{

	//		return false;
	//	}





	//}

	return false;
}
