//================================================
//
// ui処理 [ui.cpp]
// Author : 佐藤颯紀
//
//================================================

//================================================
// インクルード
//================================================
#include "uistart.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "life.h"
#include "player.h"

//================================================
//静的メンバ変数宣言
//================================================
LPDIRECT3DTEXTURE9 CUiStart::m_apTexture[UISTART_TYPE] = {};

//================================================
//クリエイト処理
//================================================
CUiStart* CUiStart::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, UISTARTTYPE type)
{
	// オブジェクトを生成
	CUiStart* pUiStart = new CUiStart;

	// 初期化処理
	pUiStart->Init(pos, size, type);

	return pUiStart;

}

//================================================
//ロード処理
//================================================
HRESULT CUiStart::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	//ready
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/READY.png", //ファイルの読み込み
		&m_apTexture[UISTARTTYPE_READY]);

	//round1
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/ROUND1.png", //ファイルの読み込み
		&m_apTexture[UISTARTTYPE_ROUND_1]);
	//round2
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/ROUND2.png", //ファイルの読み込み
		&m_apTexture[UISTARTTYPE_ROUND_2]);


	//round3
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/ROUND3.png", //ファイルの読み込み
		&m_apTexture[UISTARTTYPE_ROUND_3]);


	return S_OK;
}

//================================================
//ロードの終了
//================================================
void CUiStart::Unload(void)
{
	for (int nCount = 0; nCount < UISTART_TYPE; nCount++)
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
CUiStart::CUiStart()
{
	m_pos = (D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// 座標
	m_move = (D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// 移動量
	m_size = (D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// サイズ
	m_type = UISTARTTYPE_NONE;						// タイプ
}

//================================================
//デストラクタ
//================================================
CUiStart::~CUiStart()
{

}

//================================================
//初期化処理
//================================================
HRESULT CUiStart::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, UISTARTTYPE type)
{
	m_pos = pos;	//位置情報
	m_size = size;	//サイズ
	m_type = type;	//タイプ

	//CScene2dの初期化
	CScene2d::Init(pos, size);

	CScene2d::BindTexture(m_apTexture[type]);

	//位置設定
	CScene2d::SetPos(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z));

	//サイズ設定
	CScene2d::SetPolygonSize(D3DXVECTOR3(m_size.x, m_size.y, m_size.z));

	//カラー設定
	CScene2d::SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	return S_OK;
}

//================================================
//終了処理
//================================================
void CUiStart::Uninit(void)
{
	CScene2d::Uninit();
}

//================================================
//更新処理
//================================================
void CUiStart::Update(void)
{
	CScene2d::Update();

	for (int nCount = 0; nCount < MAX_PLAYER; nCount++)
	{
		if (CGame::GetPlayer(nCount)->GetLife(nCount)->GetReadey() == false)
		{
			Uninit();
			return;
		}
	}
}

//================================================
//描画処理
//================================================
void CUiStart::Draw(void)
{
	CScene2d::Draw();
}