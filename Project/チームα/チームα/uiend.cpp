//================================================
//
// 終了時のui処理 [uiend.cpp]
// Author : 佐藤颯紀
//
//================================================

//================================================
// インクルード
//================================================
#include "uiend.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "life.h"
#include "fade.h"

//================================================
// マクロ定義
//================================================
#define END_TIME	(200)

//================================================
//静的メンバ変数宣言
//================================================
LPDIRECT3DTEXTURE9 CUiEnd::m_apTexture[UIEND_TYPE] = {};

//================================================
//クリエイト処理
//================================================
CUiEnd* CUiEnd::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, UIENDTYPE type)
{
	// オブジェクトを生成
	CUiEnd* pUiStart = new CUiEnd;

	// 初期化処理
	pUiStart->Init(pos, size, type);

	return pUiStart;

}

//================================================
//ロード処理
//================================================
HRESULT CUiEnd::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/continue000.png", //ファイルの読み込み
		&m_apTexture[UIENDTYPE_CONTINUE]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/Thanks.png", //ファイルの読み込み
		&m_apTexture[UIENDTYPE_THANKS]);

	return S_OK;
}

//================================================
//ロードの終了
//================================================
void CUiEnd::Unload(void)
{
	for (int nCount = 0; nCount < UIEND_TYPE; nCount++)
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
CUiEnd::CUiEnd()
{
	m_pos = (D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// 座標
	m_move = (D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// 移動量
	m_size = (D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// サイズ
	m_type = UIENDTYPE_NONE;						// タイプ
	m_nCount = 0;
}

//================================================
//デストラクタ
//================================================
CUiEnd::~CUiEnd()
{

}

//================================================
//初期化処理
//================================================
HRESULT CUiEnd::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, UIENDTYPE type)
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
void CUiEnd::Uninit(void)
{
	CScene2d::Uninit();
}

//================================================
//更新処理
//================================================
void CUiEnd::Update(void)
{
	CScene2d::Update();

	// 最後のロゴの時
	if (m_type == UIENDTYPE_THANKS)
	{
		m_nCount++;

		if (m_nCount >= END_TIME)
		{
			// 終了処理
			Uninit();

			CManager::GetFade()->SetFade(CManager::MODE_TYPE_TITLE);
			return;
		}
	}
}

//================================================
//描画処理
//================================================
void CUiEnd::Draw(void)
{
	CScene2d::Draw();
}