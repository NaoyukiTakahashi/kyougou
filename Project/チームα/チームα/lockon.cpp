//================================================
//
// ロックオン処理 [lockon.cpp]
// Author : 佐藤颯紀
//
//================================================

//================================================
// インクルード
//================================================
#include "lockon.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "game.h"
#include "player.h"
#include "life.h"
#include "keyboard.h"

//================================================
//静的メンバ変数宣言
//================================================
LPDIRECT3DTEXTURE9 CLockon::m_pTexture = 0;
int CLockon::m_nLockon = 0;

//================================================
//クリエイト処理
//================================================
CLockon* CLockon::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, LOCKONTYPE type)
{
	if (m_nLockon < 2)
	{
		// オブジェクトを生成
		CLockon* pLockon = new CLockon;

		m_nLockon++;

		// 初期化処理
		pLockon->Init(pos, size, type);

		return pLockon;
	}
	return S_OK;
}

//================================================
//ロード処理
//================================================
HRESULT CLockon::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"Data/TEXTURE/Lockon.png", //ファイルの読み込み
		&m_pTexture);

	return S_OK;
}

//================================================
//ロードの終了
//================================================
void CLockon::Unload(void)
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
CLockon::CLockon()
{
	m_nLockCounter = 0;							// アニメーションのカウンター
	m_nLockPattern = 0;							// アニメーション数
	m_pos = (D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// 座標
	m_move = (D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// 移動量
	m_size = (D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// サイズ
	m_type = LOCKONTYPE_NONE;
}

//================================================
//デストラクタ
//================================================
CLockon::~CLockon()
{

}

//================================================
//初期化処理
//================================================
HRESULT CLockon::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, LOCKONTYPE type)
{
	m_pos = pos;	// 位置情報
	m_size = size;	// サイズ
	m_type = type;	// タイプ

	CScene2d::Init(pos, size);	//CScene2dの初期化

	CScene2d::BindTexture(m_pTexture);

	//位置設定
	CScene2d::SetPos(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z));

	//サイズ設定
	CScene2d::SetPolygonSize(D3DXVECTOR3(m_size.x, m_size.y, m_size.z));

	//カラー設定
	CScene2d::SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	//テクスチャー設定
	CScene2d::SetAnim(m_nLockPattern, 0.5);

	return S_OK;
}

//================================================
//終了処理
//================================================
void CLockon::Uninit(void)
{
	m_nLockon--;
	CScene2d::Uninit();
}

//================================================
//更新処理
//================================================
void CLockon::Update(void)
{
	CScene2d::Update();
	//色の取得
	D3DXCOLOR col = GetCol();
	m_nLockCounter++;
	if (m_nLockCounter == 8)
	{
		m_nLockPattern++;
		CScene2d::SetAnim(m_nLockPattern, 0.5f);

		m_nLockCounter = 0;
	}

	//ロックオン
	if (m_type == LOCKONTYPE_FAST_PLAYER)
	{
		if ((CGame::GetCamera(0)->GetTargetBool() == true))
		{
			col.a = 1.0f;
		}
		else
		{
			col.a = 0.0f;
		}
	}

	if (m_type == LOCKONTYPE_SECOND_PLAYER)
	{
		if ((CGame::GetCamera(1)->GetTargetBool() == true))
		{
			col.a = 1.0f;
		}
		else
		{
			col.a = 0.0f;
		}
	}
	//色の設定
	SetCol(col);
}

//================================================
//描画処理
//================================================
void CLockon::Draw(void)
{
	CScene2d::Draw();
}
