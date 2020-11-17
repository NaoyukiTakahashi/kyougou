//=============================================================================
//
// タイム処理 [time.cpp]
// Author : 佐藤颯紀
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "main.h"
#include "time.h"
#include "number.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "game.h"
#include "life.h"
#include "player.h"

//=============================================================================
// クリエイト関数
//=============================================================================
CTime *CTime::Create(D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	CTime *pTime = new CTime;

	pTime->Init(pos, D3DXVECTOR3(0.0f,0.0f,0.0f));

	return pTime;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CTime::CTime(int nPriority) : CScene(nPriority)
{
	m_pTexture = NULL;	// テクスチャへのポインタ
	m_pVtxBuff = NULL;	// 頂点バッファへのポインタ
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// ポリゴンの位置
	memset(m_apNumber, 0, sizeof(m_apNumber));		// 数字の配置
	m_nTime = 0;						// タイム
	m_nTimeCount = 0;					// タイムのカウンター
}

//=============================================================================
// デストラクタ
//=============================================================================
CTime::~CTime()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTime::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	m_nTimeCount = 0;

	m_nTime = TIME_LIMIT;

	for (int nCount = 0; nCount < MAX_TIME; nCount++)
	{
		if (nCount < 2)
		{
			m_apNumber[nCount] = CNumber::Create(D3DXVECTOR3(pos.x + nCount*TIME_INTERVAL, pos.y, 0.0f), D3DXVECTOR3(TIME_S_SIZE_X, TIME_S_SIZE_Y, 0.0f), CNumber::NUMBERTYPE_GAME);
		}
		else if (nCount < 3)
		{
			m_apNumber[nCount] = CNumber::Create(D3DXVECTOR3(pos.x + nCount*TIME_INTERVAL2, pos.y+ TIME_INTERVAL_Y, 0.0f), D3DXVECTOR3(TIME_MS_SIZE_X, TIME_MS_SIZE_Y, 0.0f),CNumber::NUMBERTYPE_GAME);
		}
		else
		{
			m_apNumber[nCount] = CNumber::Create(D3DXVECTOR3(pos.x + nCount*TIME_INTERVAL3, pos.y + TIME_INTERVAL_Y, 0.0f), D3DXVECTOR3(TIME_MS_SIZE_X, TIME_MS_SIZE_Y, 0.0f),CNumber::NUMBERTYPE_GAME);
		}
		
	}

	AddTime(0);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTime::Uninit(void)
{
	for (int nCount = 0; nCount < MAX_TIME; nCount++)
	{
		if (m_apNumber[nCount] != NULL)
		{
			m_apNumber[nCount]->Uninit();
			m_apNumber[nCount] = NULL;
		}
	}

	// リリース
	SetDeathFlag();


	
}

//=============================================================================
// 更新処理
//=============================================================================
void CTime::Update(void)
{
	// プレイヤーの状態情報
	CPlayer::PLAYER_STATE state[MAX_PLAYER] = 
	{ CPlayer::PLAYER_STATE_NONE, CPlayer::PLAYER_STATE_NONE };

	for (int nCount = 0; nCount < MAX_PLAYER; nCount++)
	{
		// プレイヤー情報受け取り
		if (CGame::GetPlayer(nCount) != NULL)
		{
			// 終了情報受け取り
			state[nCount] = CGame::GetPlayer(nCount)->GetState();
		}
	}


	// どちらも死んでいないなら
	if (state[0] != CPlayer::PLAYER_STATE_EXPLOSION && state[1] != CPlayer::PLAYER_STATE_EXPLOSION)
	{
		for (int nCount = 0; nCount < MAX_TIME; nCount++)
		{
			m_apNumber[nCount]->Update();
		}


		m_nTimeCount++;

		//Readyが消えたら時間が動き出す
		if (CGame::GetPlayer(0)->GetLife(0)->GetReadey() == false)
		{
			if (m_nTimeCount % 1 == 0)
			{
				if (m_nTime > 0)
				{
					AddTime(1);
				}
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CTime::Draw(void)
{
	for (int nCount = 0; nCount < MAX_TIME; nCount++)
	{
		m_apNumber[nCount]->Draw();
	}
}

//=============================================================================
// スコア加算
//=============================================================================
void CTime::AddTime(int nValue)
{
	m_nTime -= nValue;

	for (int nCount = 0; nCount < MAX_TIME; nCount++)
	{
		m_apNumber[nCount]->SetNumber(m_nTime % (int)powf(10, MAX_TIME - nCount) / powf(10, MAX_TIME - nCount - 1));
	}
}
