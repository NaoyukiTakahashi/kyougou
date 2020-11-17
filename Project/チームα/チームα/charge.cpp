//=============================================================================
// 
// 弾のチャージゲージの処理(charge.cpp)
// Author : 佐藤颯紀
//
//=============================================================================
#include "main.h"
#include "charge.h"
#include "manager.h"
#include "renderer.h"
#include "keyboard.h"
#include "life.h"

//================================================
//静的メンバ変数宣言
//================================================
LPDIRECT3DTEXTURE9 CCharge::m_pTexture = {};
float CCharge::m_fLimit[CHARGE_LIMIT] = {};

//================================================
//クリエイト処理
//================================================
CCharge* CCharge::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, D3DXCOLOR col)
{
	// オブジェクトを生成
	CCharge* pCharge = new CCharge();

	// 初期化処理
	pCharge->Init(pos, size, col);

	return pCharge;
}

//================================================
//画像の読み込み
//================================================
HRESULT CCharge::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/gage.png", //ファイルの読み込み
		&m_pTexture);

	return S_OK;
}

//================================================
//テクスチャの破棄
//================================================
void CCharge::Unload(void)
{
	//テクスチャの開放
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//================================================
//コンストラクタ
//================================================
CCharge::CCharge()
{
	m_nCounterCharge = 0;
	m_nReduce = 0;
	m_nMaxCounter = 0;
	m_bCharge = false;
	m_nBlue = 0.00f;
	m_nRed = 0.00f;
	m_nPlayerNum = 0;
}

//================================================
//デストラクタ
//================================================
CCharge::~CCharge()
{

}

//================================================
//初期化処理
//================================================
HRESULT CCharge::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, D3DXCOLOR col)
{
	m_pos = pos;		//位置情報
	m_size = size;		//サイズ
	m_col = col;		//カラー

	for (int nCount = 0; nCount < CHARGE_LIMIT; nCount++)
	{
		m_fLimit[nCount] = m_size.x;
	}

	CGauge::Init(pos, size);	//CGaugeの初期化

	CGauge::BindTexture(m_pTexture); //テクスチャーの取得

	//位置設定
	CGauge::SetPos(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z));

	//サイズ設定
	CGauge::SetSize(D3DXVECTOR3(m_size.x, m_size.y, m_size.z));

	//カラー設定
	CGauge::SetCol(D3DXCOLOR(m_col.r, m_col.g, m_col.b, m_col.a));

	return S_OK;
}

//================================================
//終了処理
//================================================
void CCharge::Uninit(void)
{
	CGauge::Uninit();
}

//================================================
//更新処理
//================================================
void CCharge::Update(void)
{
	CGauge::Update();

	// キーボード更新
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	//Readyが消えたら動き出す
	if (CLife::GetReadey() == false)
	{
		//ゲージが減る処理
		GaugeRedce();
		//ゲージを元に戻す処理
		GaugeReturn();
		//ゲージが最大の時の処理
		GaugeMax();
	}
}

//================================================
//描画処理
//================================================
void CCharge::Draw(void)
{
	CGauge::Draw();
}

//================================================
//いくつ減らすかの指定
//================================================
void CCharge::Reduce(int Counter, bool Charge,int player)
{
	m_nReduce = Counter;
	m_bCharge = Charge;
	m_nPlayerNum = player;
}

//================================================
//ゲージを減らしていく処理
//================================================
void CCharge::GaugeRedce(void)
{
	//サイズの取得
	D3DXVECTOR3 size = GetSize();
	//色の取得
	D3DXCOLOR col = GetCol();

	//ゲージを減らす処理
	if (0 < size.x)
	{
		if (m_bCharge == true)
		{
			m_nCounterCharge++;
			col = D3DCOLOR_RGBA(87, 210, 128, 255);
			size.x -= 2;

			m_fLimit[m_nPlayerNum] -= 2;

		}

		//決められたカウント分動いたらもとに戻す
		if (m_nCounterCharge == m_nReduce)
		{
			m_bCharge = false;
			m_nCounterCharge = 0;
		}
	}
	else
	{
		m_bCharge = false;
	}

	//サイズの設定
	SetSize(size);
	//色の設定
	SetCol(col);
}

//================================================
//ゲージを元に戻す処理
//================================================
void CCharge::GaugeReturn(void)
{
	//サイズの取得
	D3DXVECTOR3 size = GetSize();
	//色の取得
	D3DXCOLOR col = GetCol();

	//最大数よりゲージが減ってたら最大数まで戻す処理
//	if (size.x < MAX_CHARGE && m_bCharge == false || size.x < 0)
	if (size.x < MAX_CHARGE&& m_bCharge == false && size.x > -1500.0f)
	{
		m_nBlue = size.x / 4 * 0.01f;
		m_nRed = 1.0f - m_nBlue;

		size.x +=2;

		m_fLimit[m_nPlayerNum] +=2;

		col = D3DXCOLOR(m_nRed, 0.0f, m_nBlue, 1.0f);
		m_nMaxCounter = 0;
	}

	//サイズの設定
	SetSize(size);
	//色の設定
	SetCol(col);
}

//================================================
//ゲージが最大の時の処理
//================================================
void CCharge::GaugeMax(void)
{
	//サイズの取得
	D3DXVECTOR3 size = GetSize();
	//色の取得
	D3DXCOLOR col = GetCol();

	//最大値の時の処理
	if (size.x == MAX_CHARGE)
	{
		m_nMaxCounter++;
		//カウンターが一定数いってない間は表示
		if (m_nMaxCounter < 100)
		{
			col = D3DCOLOR_RGBA(87, 210, 128, 255);
		}

		//カウンターが一定数に行けば表示を消す
		if (m_nMaxCounter == 100)
		{
			col = D3DCOLOR_RGBA(0, 0, 0, 0);
		}
	}

	//サイズの設定
	SetSize(size);
	//色の設定
	SetCol(col);
}

//================================================
//弾の制御するプレイヤーの番号を取得する
//================================================
float CCharge::GetCharge(int nPlayer)
{
	return m_fLimit[nPlayer];
}