//================================================
//
// ui処理 [ui.cpp]
// Author : 佐藤颯紀
//
//================================================

//================================================
// インクルード
//================================================
#include "uipause.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "life.h"
#include "joypad.h"

//================================================
//静的メンバ変数宣言
//================================================
LPDIRECT3DTEXTURE9 CUiPause::m_apTexture[UISTART_TYPE] = {};

//================================================
//クリエイト処理
//================================================
CUiPause* CUiPause::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, UIPAUSETYPE type)
{
	// オブジェクトを生成
	CUiPause* pUiPause = new CUiPause;

	// 初期化処理
	pUiPause->Init(pos, size, col,type);

	return pUiPause;

}

//================================================
//ロード処理
//================================================
HRESULT CUiPause::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	//ready
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/restart.png", //ファイルの読み込み
		&m_apTexture[UIPAUSETYPE_START]);

	//round1
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/titleback.png", //ファイルの読み込み
		&m_apTexture[UIPAUSETYPE_TITLE]);

	return S_OK;
}

//================================================
//ロードの終了
//================================================
void CUiPause::UnLoad(void)
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
CUiPause::CUiPause()
{
	m_pos = (D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// 座標
	m_move = (D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// 移動量
	m_size = (D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// サイズ
	m_type = UIPAUSETYPE_NONE;						// タイプ
}

//================================================
//デストラクタ
//================================================
CUiPause::~CUiPause()
{

}

//================================================
//初期化処理
//================================================
HRESULT CUiPause::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, UIPAUSETYPE type)
{
	m_pos = pos;	//位置情報
	m_size = size;	//サイズ
	m_col = col;
	m_type = type;	//タイプ

	//CScene2dの初期化
	CScene2d::Init(pos, size);

	CScene2d::BindTexture(m_apTexture[type]);

	//位置設定
	CScene2d::SetPos(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z));

	//サイズ設定
	CScene2d::SetPolygonSize(D3DXVECTOR3(m_size.x, m_size.y, m_size.z));

	//カラー設定
	CScene2d::SetCol(D3DXCOLOR(m_col.r, m_col.g, m_col.b, m_col.a));

	//オブジェクトタイプの設定
	SetObjType(CScene::OBJTYPE_PAUSE);

	return S_OK;
}

//================================================
//終了処理
//================================================
void CUiPause::Uninit(void)
{
	CScene2d::Uninit();
}

//================================================
//更新処理
//================================================
void CUiPause::Update(void)
{
	CScene2d::Update();

	Select();
}

//================================================
//描画処理
//================================================
void CUiPause::Draw(void)
{
	CScene2d::Draw();
}

//================================================
//Selectされてる色の変化の処理
//================================================
void CUiPause::Select(void)
{
	// ジョイパッドの取得
	DIJOYSTATE js = CInputJoypad::GetStick(0);

	D3DXCOLOR col = GetCol();
	
	//入力が存在する
	if (js.lX != 0.0f || js.lY != 0.0f)
	{
		if (js.lY < -5)
		{
			if (m_type == UIPAUSETYPE_START)
			{
				col.a = 1.0f;
			}

			if (m_type == UIPAUSETYPE_TITLE)
			{
				col.a = 0.5f;
			}
		}

		if (js.lY > 5)
		{
			if (m_type == UIPAUSETYPE_START)
			{
				col.a = 0.5f;
			}

			if (m_type == UIPAUSETYPE_TITLE)
			{
				col.a = 1.0f;
			}
		}
	}

	SetCol(col);
}