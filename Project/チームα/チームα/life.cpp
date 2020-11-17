//=============================================================================
// 
// ライフゲージ処理(life.h)
// Author : 佐藤颯紀
//
//=============================================================================
#include "main.h"
#include "Life.h"
#include "manager.h"
#include "renderer.h"
#include "ui.h"
#include "sound.h"

//================================================
//静的メンバ変数宣言
//================================================
LPDIRECT3DTEXTURE9 CLife::m_apTexture[LIFETYPE_PLAYER_MAX] = {};
bool CLife::m_bStart = true;

//================================================
//クリエイト処理
//================================================
CLife* CLife::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXCOLOR col, const LIFETYPE type)
{
	// オブジェクトを生成
	CLife* pLife = new CLife;

	// 初期化処理
	pLife->Init(pos, size, col, type);

	return pLife;
}

//================================================
//画像の読み込み
//================================================
HRESULT CLife::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/Life001.png", //ファイルの読み込み
		&m_apTexture[LIFETYPE_FAST_PLAYER]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/Life002.png", //ファイルの読み込み
		&m_apTexture[LIFETYPE_SECOND_PLAYER]);

	return S_OK;
}

//================================================
//テクスチャの破棄
//================================================
void CLife::Unload(void)
{
	for (int nCount = 0; nCount < LIFETYPE_PLAYER_MAX; nCount++)
	{
		//テクスチャの開放
		if (m_apTexture[nCount] != NULL)
		{
			m_apTexture[nCount]->Release();
			m_apTexture[nCount] = NULL;
		}
	}
}
//================================================
//コンストラクタ
//================================================
CLife::CLife()
{
	m_nCounter = 0;
	m_nCounterLife = 0;
	m_nReduce = 0;
	m_nPlayerNum = 0;
	m_bLife = false;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);		//カラー
	m_type = LIFETYPE_NONE;		//タイプ
	m_bEnd= false;
	m_bStart = true;
	m_fLife = MAX_LIFE;
	m_bArmor = false;
}

//================================================
//デストラクタ
//================================================
CLife::~CLife()
{
	m_bStart = false;
}

//================================================
//初期化処理
//================================================
HRESULT CLife::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXCOLOR col, const LIFETYPE type)
{
	m_pos = pos;		//位置情報
	m_size = size;		//サイズ
	m_col = col;		//カラー
	m_type = type;		//タイプ
	m_fLife = MAX_LIFE;
	m_bArmor = false;

	CGauge::Init(pos, size);	//CGaugeの初期化

	CGauge::BindTexture(m_apTexture[type]); //テクスチャーの取得

	//位置設定
	CGauge::SetPos(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z));

	//サイズ設定
	CGauge::SetSize(D3DXVECTOR3(size.x, size.y, size.z));

	//カラー設定
	CGauge::SetCol(D3DXCOLOR(m_col.r, m_col.g, m_col.b, m_col.a));

	return S_OK;
}

//================================================
//終了処理
//================================================
void CLife::Uninit(void)
{
	CGauge::Uninit();
}

//================================================
//更新処理
//================================================
void CLife::Update(void)
{
	CGauge::Update();

	if (m_bStart == true)
	{
		//ライフの最初の動き
		LifeStart();
	}
	else
	{
		//ライフを減らす
		Lifereduce();

		//ライフが半分になると点滅させる
		LifeFlashing();
	}
}

//================================================
//描画処理
//================================================
void CLife::Draw(void)
{
	CGauge::Draw();
}

//================================================
//ライフをいくつ減らすかの取得・ヒット！を出す
//================================================
void CLife::Decrease(int Reduce, int PlayerNamber, bool Life)
{
	if (m_bArmor == false)
	{
		m_fLife -= (float)Reduce;
		m_bArmor = true;
	}
	m_nReduce = Reduce;
	m_bLife = Life;
	m_nPlayerNum = PlayerNamber;

	//２Ｐに弾があたったら１Ｐの場所にＨＩＴの文字を出す
	if (m_nPlayerNum == 0)
	{
		CUi::Create(D3DXVECTOR3(UI_HIT_POS_LEFT_X, 600.0f, 0.0f), D3DXVECTOR3(UI_HIT_SIZE_X, UI_HIT_SIZE_Y, 0.0f), CUi::UITYPE_HIT);
	}
	//１Ｐに弾があたったら２Ｐの場所にＨＩＴの文字を出す
	if (m_nPlayerNum == 1)
	{
		CUi::Create(D3DXVECTOR3(UI_HIT_POS_RIGHT_X, 600.0f, 0.0f), D3DXVECTOR3(UI_HIT_SIZE_X, UI_HIT_SIZE_Y, 0.0f), CUi::UITYPE_HIT);
	}
}

//================================================
//ライフを減らす処理
//================================================
void CLife::Lifereduce(void)
{
	CSound *pSound = CManager::GetSound();

	//サイズの取得
	D3DXVECTOR3 size = GetSize();
	//色の取得
	D3DXCOLOR col = GetCol();

	//ライフを減らす処理
	if (0 < size.x)
	{

		if (m_bLife == true)
		{
			pSound->Play(CSound::SOUND_LABEL_SE_BOMB);

			m_nCounterLife++;

			size.x--;

			//自分のライフの色が変わったとき
			if (m_type == LIFETYPE_FAST_PLAYER)
			{
				col = D3DCOLOR_RGBA(255, 0, 0, 255);
			}
			//相手のライフの色が変わったとき
			if (m_type == LIFETYPE_SECOND_PLAYER)
			{
				col = D3DCOLOR_RGBA(0, 50, 255, 255);
			}

			//決められたカウント分動いたらもとに戻す
			if (m_nCounterLife == m_nReduce)
			{
				m_bLife = false;
				m_nCounterLife = 0;

				m_bArmor = false;

				//自分のライフの色を元に戻す
				if (m_type == LIFETYPE_FAST_PLAYER)
				{
					col = D3DCOLOR_RGBA(255, 255, 255, 255);
				}
				//相手のライフの色を元に戻す
				if (m_type == LIFETYPE_SECOND_PLAYER)
				{
					col = D3DCOLOR_RGBA(255, 255, 255, 255);
				}
			}
		}
	}
	else
	{
		m_bStart = true;
		size.x = 0.0f;
	}


	//サイズの設定
	SetSize(size);
	//色の設定
	SetCol(col);
}

//================================================
//ライフが半分になると点滅させる
//================================================
void CLife::LifeFlashing(void)
{
	//サイズの取得
	D3DXVECTOR3 size = GetSize();
	//色の取得
	D3DXCOLOR col = GetCol();

	//ライフが半分以下になると点滅するようにする
	if (size.x < HALF_LIFE)
	{
		m_nCounter++;

		if (m_nCounter == 15)
		{
			col.a = 0.8f;
		}
		else if (m_nCounter == 30)
		{
			col.a = 1.0f;

			m_nCounter = 0;
		}
	}

	//サイズの設定
	SetSize(size);
	//色の設定
	SetCol(col);
}

//================================================
//ゲームスタート時のライフの動き
//================================================
void CLife::LifeStart(void)
{
	//サイズの取得
	D3DXVECTOR3 size = GetSize();

	if (m_size.x < MAX_LIFE)
	{
		if (m_bStart == true)
		{
			m_size.x += 2;
		}
	}
	else
	{
		m_bStart = false;
	}
	//サイズの設定
	SetSize(m_size);
}

//================================================
// READYの設定
//================================================
void CLife::SetReady(bool bReady)
{
	m_bStart = bReady;
}

//================================================
//READYの使用してるかしてないかの取得
//================================================
bool CLife::GetReadey(void)
{
	return m_bStart;
}


float CLife::GetLife(void)
{
	return m_fLife;
}

bool CLife::GetbLife(void)
{
	return m_bLife;
}
