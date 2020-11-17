//=====================================================
//
// スプラッシュクラス [splash.cpp]
// Author : Konishi Yuuto
//
//=====================================================

//=====================================================
// インクルード
//=====================================================
#include "splash.h"
#include "manager.h"
#include "renderer.h"

//=====================================================
// マクロ定義
//=====================================================
#define SPLASH_ANIME_SPEED		(5)		// アニメーションの速さ
#define SPLASH_ANIME_PATTERN	(8)		// アニメーションのパターン数

//=====================================================
// static初期化s
//=====================================================
LPDIRECT3DTEXTURE9 CSplash::m_pTexture = NULL;			// ポリゴンのテクスチャ

//=====================================================
// ポリゴン生成
//=====================================================
CSplash * CSplash::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// メモリ確保
	CSplash *pSplash = new CSplash;

	if (pSplash != NULL)
	{
		// 初期化処理
		pSplash->Init(pos, size);
	}

	return pSplash;
}

//=====================================================
// コンストラクタ
//=====================================================
CSplash::CSplash()
{

}

//=====================================================
// デストラクタ
//=====================================================
CSplash::~CSplash()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CSplash::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	// 初期化処理
	CBillboard::Init(pos, size);

	// テクスチャ設定
	BindTexture(m_pTexture);

	// アニメーション設定
	InitAnimation(SPLASH_ANIME_SPEED, SPLASH_ANIME_PATTERN, 0);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CSplash::Uninit(void)
{
	// 終了処理
	CBillboard::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CSplash::Update(void)
{
	// 更新処理
	CBillboard::Update();
}

//=====================================================
// 描画処理
//=====================================================
void CSplash::Draw(void)
{
	// レンダラーの情報を受け取る
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// かぶさらないようにする　(Zバッファ)
//	pDevice->SetRenderState(D3DRS_ZENABLE, false);

	// 加算合成を行う
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			// aデスティネーションカラー

	D3DMATERIAL9 material;
	ZeroMemory(&material, sizeof(D3DMATERIAL9));
	material.Ambient.r = 1.0f;
	material.Ambient.g = 1.0f;
	material.Ambient.b = 1.0f;
	material.Ambient.a = 1.0f;
	pDevice->SetMaterial(&material);
	pDevice->SetRenderState(D3DRS_AMBIENT, 0x44444444);

	DWORD ambient;
	pDevice->GetRenderState(D3DRS_AMBIENT, &ambient);
	pDevice->SetRenderState(D3DRS_AMBIENT, 0xffffffff);
	pDevice->LightEnable(0, FALSE);

	// アルファテストを有力化
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	// アルファテスト基準値の設定
	pDevice->SetRenderState(D3DRS_ALPHAREF, 75);

	// 2Dポリゴン描画処理
	CBillboard::Draw();

	// アルファテストを有力化
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// アルファテスト基準値の設定
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// aデスティネーションカラー

	// Zバッファ戻す
//	pDevice->SetRenderState(D3DRS_ZENABLE, true);

	pDevice->SetRenderState(D3DRS_AMBIENT, ambient);
	pDevice->LightEnable(0, TRUE);

}

//=====================================================
// テクスチャロード
//=====================================================
HRESULT CSplash::Load(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/Texture/splash.png", &m_pTexture);
	return S_OK;
}

//=====================================================
// テクスチャアンロード
//=====================================================
void CSplash::UnLoad(void)
{
	// テクスチャの開放
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}