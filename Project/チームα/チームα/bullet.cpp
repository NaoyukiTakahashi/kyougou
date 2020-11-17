//=============================================================================
//
// 弾の処理 [bullet.cpp]
// Author : 山田陵太
//
//=============================================================================
#include "bullet.h"
#include "player.h"
#include "game.h"
#include "life.h"
#include "2d_explosion.h"
#include "shock.h"
#include "manager.h"
//=============================================================================
//マクロ定義
//=============================================================================
#define BULLET_LIFE 100	//バレットライフ 
#define BULLET_ATK 50	//攻撃力
#define BULLET_SPEED 50.0f	//速度

//=============================================================================
//バレットクラスのコンストラクタ
//=============================================================================
CBullet::CBullet()
{
	//各メンバ変数のクリア
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//位置
	m_move = D3DXVECTOR3(10.0f, 0.0f, 0.0f);	//移動量
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//サイズ
	m_user = BULLET_USER_NONE;	//使用者
	m_nAtk = 0;	//攻撃力
	m_nLife = 0;	//ライフ
	m_nCounter = 0;
	m_pTargetPL = NULL;	//敵プレイヤーのポインタ
}

//=============================================================================
//バレットクラスのデストラクタ
//=============================================================================
CBullet::~CBullet()
{
}

//=============================================================================
//バレットクラスのクリエイト処理
//=============================================================================
CBullet * CBullet::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const BULLET_USER user)
{
	CBullet *pBullet = NULL;

	//メモリの確保
	pBullet = new CBullet;

	//メモリが確保できていたら
	if (pBullet)
	{
		//初期化処理呼び出し
		pBullet->Init(pos, size, user);
	}
	else
	{
		return NULL;
	}

	return pBullet;
}

//=============================================================================
//バレットクラスの初期化処理
//=============================================================================
HRESULT CBullet::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const BULLET_USER user)
{
	//位置の設定
	m_pos = pos;

	//サイズの設定
	m_size = size;

	//使用者の設定
	m_user = user;

	//ライフの設定
	m_nLife = BULLET_LIFE;

	//攻撃力の設定
	m_nAtk = BULLET_ATK;

	//ビルボードの初期化
	CBillboard::Init(m_pos, m_size);
	
	//オブジェクトタイプを設定
	//SetObjType(CScene::OBJTYPE_BULLET);

	switch (m_user)
	{
	case BULLET_USER_PL1:
		//2Pの情報取得
		m_pTargetPL = CGame::GetPlayer(1);

		break;
	case BULLET_USER_PL2:
		//1Pの情報取得
		m_pTargetPL = CGame::GetPlayer(0);

		break;
	}

	return S_OK;
}

//=============================================================================
//バレットクラスの終了処理
//=============================================================================
void CBullet::Uninit(void)
{
	//ビルボードの終了処理呼び出し
	CBillboard::Uninit();
}

//=============================================================================
//バレットクラスの更新処理
//=============================================================================
void CBullet::Update(void)
{
	//位置の取得
	m_pos = GetPos();

	if (m_nCounter <= 120)
	{
		//移動量の計算
		m_move = VectorMath(m_pTargetPL->GetPos(), BULLET_SPEED);
	}

	m_pos += m_move;

	//位置の設定
	SetPos(m_pos);

	if (Collision() == true)
	{
		Uninit();
		return;
	}

	//ライフが0以下の時
	if (m_nLife <= 0)
	{
		CShock::Create(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXVECTOR3(SHOCK_SIZE_X, SHOCK_SIZE_Y, SHOCK_SIZE_Z));
		//終了処理呼び出し
		Uninit();
		return;
	}

	//ライフの減少
	m_nLife--;
	m_nCounter++;
}

//=============================================================================
//バレットクラスの描画処理
//=============================================================================
void CBullet::Draw(void)
{
	//ビルボードの描画処理呼び出し
	CBillboard::Draw();
}

//=============================================================================
//バレットクラスの当たり判定
//=============================================================================
bool CBullet::Collision(void)
{
	D3DXVECTOR3 targetPos = m_pTargetPL->GetPos();

	if (targetPos.x >= m_pos.x - m_size.x / 2 &&
		targetPos.x <= m_pos.x + m_size.x / 2 &&
		targetPos.y >= m_pos.y - m_size.y / 2 &&
		targetPos.y <= m_pos.y + m_size.y / 2 &&
		targetPos.z >= m_pos.z - m_size.x / 2 &&
		targetPos.z <= m_pos.z + m_size.x / 2)
	{
		for (int nCount = 0; nCount < LIFE_NUM; nCount++)
		{
			//　プレイヤーのライフを減らす

			m_pTargetPL->GetLife(nCount)->Decrease(50, m_user, true);
		}

		C2dExplosion::Create(m_pos,
			D3DXVECTOR3(EXPLOSION_SIZE_X_2D, EXPLOSION_SIZE_Y_2D, EXPLOSION_SIZE_Z_2D));
		return true;
	}
		
	return false;
}
//=============================================================================
//バレットクラスののベクトル計算処理
//=============================================================================
D3DXVECTOR3 CBullet::VectorMath(D3DXVECTOR3 TargetPos, float fSpeed)
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
